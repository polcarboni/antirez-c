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
#include <assert.h>


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


// Pointer to structure, avoid the compiler to resolve to int.
struct tfctx;
/**
 * Function table entry: each one of these entry represents a symbol name
 * associated with a function implementation.
 */
typedef struct FunctionTableEntry{
	tfobj *name;
	// Callback implement different functions
	void (*callback) (struct tfctx *ctx, tfobj *name);
	tfobj *user_func;
} tffuncentry;

struct FunctionTable {
	tffuncentry **func_table;
	size_t func_count;
};

/**
 * Toy Forth execution context.
 */
typedef struct tfctx {
	tfobj *stack;
	struct FunctionTable funcTable;
} tfctx;


/* =========================== Function prototypes =========================== */
void retain(tfobj *o);
void release(tfobj *o);

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

/**
 * Memory cheked realloc
 */
void *xrealloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
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


// Reference count management

/**
 * Free an object and all the other nested objects
 */
void freeObject(tfobj *o) {
	switch(o->type) {
		case TFOBJ_TYPE_LIST:
			for (size_t j = 0; j < o->list.len; j++) {
				tfobj *ele = o->list.ele[j];	//Object from the list
				release(ele);		// Decrease reference count (can have more elements)
			}
			break;
		case TFOBJ_TYPE_SYMBOL:
		case TFOBJ_TYPE_STR:
			free(o->str.ptr);
			break;
	}
	free(o);			// Free the object reference (every type)
}

void retain(tfobj *o) {
	o->refcount++;
}

void release(tfobj *o) {
	assert (o->refcount > 0);
	o->refcount--;
	if (o->refcount == 0) freeObject(o);
}


/**
 * Prints the objects or programs 
 */
void printObject(tfobj *o) {
	switch(o->type) {
		case TFOBJ_TYPE_INT:
			printf("%d", o->i);
			break;
		case TFOBJ_TYPE_LIST:
			printf("[");
			for (size_t j = 0; j < o->list.len; j++) {
				tfobj *ele = o->list.ele[j];	//Object from the list
				
				printObject(ele);		// Recursive call to print all the list objects
				
				// Print space between objects (not on the last one) 
				if (j != o->list.len-1) {
					printf(" ");
				}
			}
			printf("]");
			break;
		case TFOBJ_TYPE_SYMBOL:
			printf("%s", o->str.ptr);
			break;
		case TFOBJ_TYPE_STR:
			printf("\"%s\"", o->str.ptr);
			break;
		default:
			printf("?");
	}
}

/* =================== String object ==================== */

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

/**
 * Compare the two string objects 'a' and 'b'.
 * Returns 0 if they are the same, '1' if a>b, '-1' if a<b.
 * The comparison is performed using memcmp().
 */ 
int compareStringObject(tfobj *a, tfobj *b) {
	size_t minlen = a->str.len < b->str.len ? a->str.len : b->str.len;
	
	//memcmp: Treat void ptrs like bytes, compare same sized strings
	int cmp = memcmp(a->str.ptr, b->str.ptr, minlen); 

	if (cmp == 0) {
		if (a->str.len == b->str.len) return 0;
		else if (a->str.len > b->str.len) return 1;
		else return -1;
	} else {
		if (cmp < 0) return -1;
		else if (cmp > 0) return 1;
	}
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
	l -> list.ele = xrealloc(l->list.ele, sizeof(tfobj*) * (l->list.len+1)); //Edit size of previous allocation
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
int isSymbolChar(int c) {
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
	while(parser->p[0] && isSymbolChar(parser->p[0])) parser->p++;
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
		} else if (isSymbolChar(parser.p[0])) {
			o = parseSymbol(&parser);
		} else {
			o = NULL;
		}

		// Checks if the current token produces a parsing error,
		// pushes the object in the program list if there is no error.
		if (o == NULL) {
			release(parsed); 
			printf("Syntax error near: %32s ...\n", token_start);
			return NULL;
		} else {
			// printf("Compiler pointer: %p\n", o); //pointer debug
			listPush(parsed, o);
		}
	}

	
	return parsed;
}

/* =========================== Basic STL =========================== */

void BasicMathFunctions(tfctx *ctx, tfobj * name) {
	
	if(ctxCheckStackMinLen(ctx,2)) return;
	
	tfobj *b = ctxStackPop(ctx, TFOBJ_TYPE_INT);
	tfobj *a = ctxStackPop(ctx, TFOBJ_TYPE_INT);  // Last element of the stack
	
	if (a == NULL || b == NULL) return;

	int result;
	switch(name->str.ptr[0]) {
		case '+': result = a->i + b->i;
		case '-': result = a->i - b->i;
		case '*': result = a->i * b->i;
	}
	
	ctxStackPush(ctx, createIntObject(result));
}


/* =========================== Execution and Context =========================== */

tffuncentry *getFunctionByName(tfctx *ctx, tfobj *name) {
	for(size_t j = 0; j < ctx ->funcTable.func_count; j++) {
		tffuncentry *fe = ctx->funcTable.func_table[j];
		
		// Function is found 
		if (compareStringObject(fe->name, name) == 0)
			return fe;
	}
	return NULL;

	ctx->funcTable.func_count = 0;
}


/**
 * Push a new function entry in the context.
 * It is up to the caller to set either the C callback or the list
 * representing the user defined function.
 */
tffuncentry *registerFunction(tfctx *ctx, tfobj *name) {
	// Array of poitner to function entries
	ctx->funcTable.func_table = xrealloc(ctx->funcTable.func_table,
										 sizeof(tffuncentry*)*(ctx->funcTable.func_count+1));
	tffuncentry *fe = xmalloc(sizeof(tffuncentry));
	ctx->funcTable.func_table[ctx->funcTable.func_count] = fe;
	ctx->funcTable.func_count++;
	fe->name = name;
	retain(name);
	fe->callback = NULL;
	fe->user_func = NULL;
	return(fe);
}

/**
 * Register a new function with the given name in the function table of the context.
 * The function can't fail since if a function with the same name already exist it gets replaced by the new one.
 */
void registerCFunction(tfctx *ctx, char *name,
					   void (*callback) (tfctx *ctx, tfobj *name)) {
	tffuncentry *fe;	
	tfobj *oname = createStringObject(name, strlen(name));
	fe = getFunctionByName(ctx, oname);
	
	if (fe) {
		if (fe->user_func) {
			release(fe->user_func);
			fe->user_func = NULL;	// either user defined func or C callback is NULL for the name
		}
		fe -> callback;  // Callback from C function
	} else {
		fe = registerFunction(ctx, oname);
		fe->callback = callback;
	}

	release(oname);
}

/**
 * Creates the execution context: jolds a list as a stack for the programming language.
 */
tfctx *createContext(void) {
	tfctx *ctx = xmalloc(sizeof(*ctx));
	ctx->stack = createListObject();
	ctx->funcTable.func_table = NULL;
	ctx->funcTable.func_count = 0;
	registerFunction(ctx, "+", basicMathFunctions);	//executes callback on different functions
	return ctx;
}

void registerFunction(tfctx *ctx, char *name, basicMathFunctions) {

}

/**
 * Tries to resolve and call the function associated with the symbol named 'word'.
 * Returns 0 if the symbol is bound to some function and was executed.
 * return 1 otherwise (on error).
 */
int callSymbol(tfctx *ctx, tfobj *word) {
	tffuncentry *fe = getFunctionByName(ctx, word);
	if (fe == NULL) return 1;	//Should set context error
	return 0;
}

/**
 * Execute the toy Forth program
 */
void exec(tfctx *ctx, tfobj *prg) {
	assert(prg->type == TFOBJ_TYPE_LIST);		// Program must be a list of objects 

	for (size_t j = 0; j<prg->list.len; j++) {
		tfobj *word = prg->list.ele[j];			// Read program words
		switch(word->type) {
			case TFOBJ_TYPE_SYMBOL:				// Execute functions
				break;
			default:							// Add object to the stack
				listPush(ctx->stack, prg->list.ele[j]);
				retain(word);					// Handles reference count
		}
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
	
	// Print the program
	printObject(prg);
	printf("\n");

	// Define the execution context, print its content
	tfctx *ctx = createContext();
	exec(ctx, prg);

	printf("Stack context at end: ");
	printObject(ctx->stack);			// Prints only the element saved in the stack (no functions)
	printf("\n");

	return 0;
}

/**
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

/**
 * Lesson 26
 * 
 * Quoted symbols are a way for programming langauges to use function names as variables. Marking them with an apex: 'print
 * can be useful to avoid the interpreter to try to execute it and use it instead as an input for another funtion.
 * (Useful for function metadata).
 * 
 * In this case we can use it to push the symbol to the stack.
 * 
 * 
 * Exec implementation:
 * 
 * Assert
 * Program stops if the expression is false returning a descriptive error.
 * Assert is a system invariant.
 * 
 * Function table:
 * Record in the execution context a serie of function that can be called.
 * Calling a symbol/function executes the scan of the function table. (callbacks).
*/

/**
 * Lesson 27
 * 
 * Implementation of the function table.
 * 
 * The function can be a callback to C function or be user defined functions. Defining a function of the same name of one
 * previously existing within the function table will not throw an error and will overwrite the function instead.
 */