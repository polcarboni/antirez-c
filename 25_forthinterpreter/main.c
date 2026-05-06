/*
 * 	-------------------------- FORTH LANGUAGE INTRODUCTION --------------------------
 * 
 * Forth is a stack-based language, in a simplified example version two examples of forth code are:
 *
 * 	1: 5 5 +
 * 	2: 5 dup +
 * 
 * Programs are list of words, if a word is a value is appended to the stack. Function words takes the last stack in the values
 * and insert the result in the stack. The functions in this 2 example programs are:
 *
 * Values needs to be in the stack before the operator for it to be used.
 *
 *  +: sum the last two values in the stack
 *  dup: duplicates the last value 
 *
 *
 * 	3: "Hello world" strlen print
 *
 *  strlen: gets the last element in the stack and put its length
 *  print: stdout stack print
 *
 *
 *  4: [dup *][dup +][10 20 <] if
 *
 *  First value is a list, put in the stack. When if is called, it gets all the elements from the stack and selects the first subprogram
 *  if the condition is verified (the second one otherwise).
 *
 *  An alternative way to write the program in a formatted way would be:
 *
 *	4: [10 20 <]
 *	  	[dup *]
 *	  	[dup +] ifelse
*/

/*
 * -------------------------- INTERPRETER SPECIFICATIONS --------------------------
 *
 * In interpreted languages, objects are important for representing each element.
 * 
 * Parser:
 * returns the current parsing location using two pointers:
 *  *prg to the program start, one for relative position 
 *
 * current code and object are also stored int the object class.
 * (Helps detecting compile and runtime errors by showing the error line).
 * 
 * Stack is represented with the same list implementation (with an array).
 * 	Not with a list because of a bad cache locality and memory usage. Allocation overhead.
 * (High level language implementation might differ from C-level interpreter implementation.)
 * 
 * By also adding an alloc_len param we can also allocate the list:
 * 	use realloc if the size is too big. List size and whole program size decoupling.
 * 	Allocating to the power of 2 helps reducing the reallocation operation number.
 * 
 * This approach allows a constant ammortized time complexity O(1).
 * 
 * Same complexity of the linked list with:
 * - Better locality
 * - Constant time access O(1) using array.
 *
*/


// TOY FORTH INTERPRETER
// Literal types that are single words (like functions) are called symbols.
// The list type tfobj can be used as the program stack .

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


/* =========================== Structures =========================== */

#define TFOBJ_TYPE_INT  0
#define TFOBJ_TYPE_STR  1
#define TFOBJ_TYPE_BOOL 2
#define TFOBJ_TYPE_LIST 3
#define TFOBJ_TYPE_SYMBOL 4

// Toy Forth objects (different syntax elements) 
typedef struct tfobj {
	int refcount;
	int type; //TFOBJ_TYPE_*
	
	union {
		//INT,BOOL	
		int i; 
		
		//STR, SYMBOL
		struct str { 
			char *ptr;
			size_t len;
		} str;

		//LIST
		struct list {
			struct tfobj **ele; //Pointers to tfobjects pointers
			size_t len;
			size_t alloc_len;
		} list;
	};
} tfobj;

// Parser
typedef struct tfparser {
	char *prg;		// Program to be compiled into a list.
	char *p; 		// Next token to parse.
} tfparser;


// Different toyforth interpreters should have separate stacks (execution contexts)
typedef struct tfctx {
	tfobj *stack;
} tfctx;

// /*Print tfobject for debugging purpose*/
// void printtfobj(tfobj *o) {
// 	printf("Type: %d, ", o->type);
	
// 	switch(o->type) {
// 	case TFOBJ_TYPE_INT:
// 		printf("%d", o->i);
// 		break;

// 	case TFOBJ_TYPE_STR:
// 		printf("%d", o->str);
// 		break;
	
// 	case TFOBJ_TYPE_BOOL:
// 		printf("%d", o->i);
// 		break;

// 	case TFOBJ_TYPE_LIST:
// 		printf("%d", o->i);
// 		break;
	
// 	case TFOBJ_TYPE_SYMBOL:
// 		printf("%d", o->str);
// 		break;
	
// 	default:
// 		printf("?");
// 		break;
// 	}
// }
/* =========================== Allocation wrappers =========================== */

//malloc with out of memory check
void *xmalloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		fprintf(stderr, "Out of memory allocating %zu bytes\n", size);
		exit(1);
	}
	return ptr;
}

/* =========================== Object related functions ===========================
 *   Allocator functions for different Toy Forth Object types.
*/

// Allocate and initialize a new Toy Forth Object.
tfobj *createObject(int type) {
	tfobj *o = xmalloc(sizeof(tfobj));
	o->type = type;
	o->refcount = 1;
	return o;
} 

tfobj *createStringObject(char *s, size_t len) {
	tfobj *o = createObject(TFOBJ_TYPE_STR);
	o->str.ptr = xmalloc(len+1);
	o->str.len = len;

	memcpy(o->str.ptr, s, len);
	o->str.ptr[len] = 0;      //Null term at the end of the symbol
	return o;
}

tfobj *createSymbolObject(char *s, size_t len) {
	tfobj *o = createStringObject(s, len);
	o->type = TFOBJ_TYPE_SYMBOL;
	return o;
}

tfobj *createIntObject(int i) {
	tfobj *o = createObject(TFOBJ_TYPE_INT);
	o->i = i;
	return o;
}

tfobj *createBoolObject(int i) {
	tfobj *o = createObject(TFOBJ_TYPE_BOOL);
	o->i = i;
	return o;
}


/* =================== List object ==================== */

tfobj *createListObject() {
	tfobj *o = createObject(TFOBJ_TYPE_LIST);
	o->list.ele = NULL;
	o->list.len = 0;
	return o;
}

/**
 * Add the new element at the end of the list 'list'.
 * It is up to the caller to increment the reference count of the
 * added element to the list if needed.
*/
void listPush(tfobj *l, tfobj *ele) {
	l -> list.ele = realloc(l->list.ele, sizeof(tfobj*) * (l->list.len+1)); //Edit size of previous allocation
	l->list.ele[l->list.len] = ele;
	l->list.len++;
}

/* =================== Turn program into toy forth list ==================== */

// Compile turns a string into a list using a parser

void parseSpaces(tfparser *parser) {
	while(isspace(parser->p[0])) parser->p++;
}

#define MAX_NUM_LEN 128
tfobj *parseNumber(tfparser *parser) {
	char buf[MAX_NUM_LEN];
	char *start = parser->p;
	char *end;

	// Read number
	if (parser->p[0] == '-') parser->p++;
	while(parser->p[0] && isdigit(parser->p[0])) parser->p++;
	
	end = parser->p; //First non-num char
	int numlen = end - start; 

	if (numlen >= MAX_NUM_LEN) return NULL;

	memcpy(buf, start, numlen);
	buf[numlen] = 0;
	// printf("DEBUG: %s\n", buf); //Correct

	tfobj *o = createIntObject(atoi(buf)); //atoi (syscall): create int from char[]
	return o;
}

/**
 * Return true if the character 'c' is one of the accetable characters
 * for the symbols.
*/ 
int is_symbol_char(int c) {
	char symchars[] = "+-*/%";

	// return isalpha(c) || strchr(symchars,c) != NULL;
	if(isalpha(c)) {
		return 1;
	} else if(strchr(symchars,c) != NULL) {
		return 1;
	} else {
		return 0; 
	}
	
}

/**
 * Parse symbol until the characters are valid for symbols (alpha or defined chars) 
 */
tfobj *parseSymbol(tfparser *parser) {
	char *start = parser->p;
	while(parser->p[0] && is_symbol_char(parser->p[0])) parser->p++;
	char *end = parser->p;
	int len = parser->p - start;
	return createSymbolObject(start, len);
}

tfobj *compile(char *prg) {
	tfparser parser;
	parser.prg = prg;
	parser.p = prg;

	tfobj *parsed = createListObject();

	while(parser.p) {
		tfobj *o;
		char *token_start = parser.p;

		
		// Check if the parsed element is a NUM or SYMBOL TYPE
		parseSpaces(&parser);
		if (parser.p[0] == 0) break; // End of program reached. 
		
		if (isdigit(parser.p[0]) || 
			(parser.p[0] == '-' && isdigit(parser.p[1])))
		{
			o = parseNumber(&parser);
		} else if (is_symbol_char(parser.p[0])) {
			o = parseSymbol(&parser);
		} else {
			o = NULL;
		}

		// Checks if the current token produces a parsing error,
		// pushes the object in the program list if there is no error.
		if (o == NULL) {
			// FIXME: release parsed here.
			printf("Syntax error near: %32s ...\n", token_start);
			return NULL;
		} else {
			// printf("Compiler pointer: %p\n", o); //pointer debug
			listPush(parsed, o);
		}
	}

	
	return parsed;
}


/* =========================== Exec function =========================== */

/**
 * Prints the obbjects or programs 
 */
void print_object(tfobj *o) {
	switch(o->type) {
		case TFOBJ_TYPE_INT:
			printf("%d", o->i);
			break;
		case TFOBJ_TYPE_LIST:
			printf("[");
			for (size_t j = 0; j < o->list.len; j++) {
				tfobj *ele = o->list.ele[j];	//Object from the list
				
				print_object(ele);		// Recursive call to print all the list objects
				printf(" ");
			}
			printf("]");
			break;
		case TFOBJ_TYPE_SYMBOL:
			printf("%s", o->str.ptr);
			break;
		
		default:
			printf("?");
	}
}

/* =========================== Main =========================== */

int main(int argc, char **argv) {
	
	if (argc != 2) {
		fprintf(stderr, "ERROR: Usage: %s <filename>\n", argv[0]);    //fprintf function: Prints to file
		return 1;
	}
	
	// printf("%s %d\n", argv[0], argc);   //argv[0]: program name, argc: args count (minimum 0) 

	// ----------------- Read the program in memory for later parsing -----------------

	printf("READ PROGRAM\n");
	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL) {
		perror("Opening toy forth program");
		return 1;
	}

	fseek(fp, 0, SEEK_END);		// Change reading location of file: goes from last to first
	long file_size = ftell(fp);	// Check reached position
	printf("Source file len: %ld\n", file_size);
	char *prgtext = xmalloc(file_size + 1);	// Updates the current location pointer using file size
	
	fseek(fp,0,SEEK_SET);		// Reset file stream to initial position and read
	fread(prgtext, file_size, 1, fp);

	prgtext[file_size] = 0;		// Append null term to the program.
	printf("Program text: %s\n", prgtext);
	fclose(fp);

	// Compile the program
	tfobj *prg = compile(prgtext);
	
	// Execute the program (dummy execution)
	print_object(prg);

	return 0;
}

/*
 * Stack based language modification proposal
 *
 * Addition of a local stack for variables, parallel to the program stack:
 * 
 *  	5 (a) $a $a
 *
 * Stack: 5
 * Local: a=5
 * 
 * $ symbol pushes the value in the program stack.
 * 
 * SWAP implementation for inverting the stack can be defined as:
 * 
 * 		5 10 (a b) $b $a
*/





//     MINUTO 35