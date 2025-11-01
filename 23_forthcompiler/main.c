/*
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


// TOY FORTH INTERPRETER
// Literal types that are single words (like functions) are called symbols.
// The list type tfobj can be used as the program stack .

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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



/* =========================== Allocation wrappers =========================== */

//malloc with out of memory check
void *xmalloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		fprintf(stderr, "Out od memory allocating %zu bytes\n", size);
		exit(1);
	}
	return ptr;
}

/* =========================== Object related functions ===========================
 *   Allocator functions for different TOy Forth Object types.
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
	o->str.ptr = s;
	o->str.len = len;
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

tfobj *createListObject(int i) {
	tfobj *o = createObject(TFOBJ_TYPE_LIST);
	o->list.ele = NULL;
	o->list.len = 0;
	return o;
}


/* =========================== Main =========================== */

int main(int argc, char **argv) {
	
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);    //fprintf function: Prints to file
		return 1;
	}
	
	printf("%s %d\n", argv[0], argc);   //argv[0]: program name, argc: args count (minimum 0) 


	//Following functions need to be implemented
	
	// tfobj *prg = compile(prgrext);
	// exec(prgtext);

	return 0;
}