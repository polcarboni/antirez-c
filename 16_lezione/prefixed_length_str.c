#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct pls {
    uint32_t len;
    uint32_t refcount;
#ifdef PLS_DEBUG    
    uint32_t magic; // 4 more bytes for safety
#endif
    char str[];
};

/*
    Initialize a prefixed length string with the specified string in 'init'
    of length 'len'.

    The created strings have the following layout:

    +----+----+---------------\\\
    |LLLL|CCCC|my string here!...
    +----+----+---------------\\\

    Where L is 4 bytes stating the total length of the string.
    Thus this strings are binary safe: zero bytes are permitted in the middle.
    
    Warning: this function does not check for buffer overflows.
*/

char *ps_create(char *init, int len) {

    struct pls *p = malloc(sizeof(struct pls) + len + 1);
    p->len = len;
    p->refcount = 1; // Refcount intialized 1 when created.

#ifdef PLS_DEBUG 
    p->magic = 0xDEADBEEF;
#endif

    for(int j = 0; j < len; j++) {
        p->str[j] = init[j]; //Should use memcopy() here.
    }
    p->str[len] = 0; //Add terminator char.
    return p->str;
}

/*  Display the string 's' on the screen. */
void ps_print(char *s) {
    struct pls *p = (struct pls*)(s - sizeof(*p));

    for(int j=0; j < p->len; j++) {
        putchar(p->str[j]);
    }
    printf("\n");
}

/* Free a previously created PS string. */
void ps_free(char *s) {
    free(s - sizeof(struct pls));
}

void ps_validate(struct pls *p) {
    #ifdef PLS_DEBUG
    if(p->magic != 0xDEADBEEF) {
        printf("INVALID STRING: Aborting\n");
        exit(1);
    }
    #endif
}

/* Decrease the reference count of string by 1. Free the memory if the last ref is deallocated. */
void ps_release (char *s) {
    struct pls *p = (struct pls*)(s - sizeof(*p));
    ps_validate(p);
    
    if (p->refcount == 0) {
        printf("ABORTED ON FREE ERROR\n");
        exit(1);
    }

    p -> refcount--;
    if (p->refcount == 0) {
        
        #ifdef PLS_DEBUG
        p->magic = 0;
        #endif
        
        ps_free(s);
        printf("Current refcount is %d\n", (int)p->refcount);
    }
}

/* Increase the reference count of the string object. */
void ps_retain(char *s) {
    struct pls *p = (struct pls*)(s - sizeof(*p));
    if (p->refcount == 0) {
        printf("ABORTED ON RETAING OF ILLEGAL STRING\n");
        exit(1);
    }
    p->refcount++;
}

/* Return the length of the string in O(1) time (instead of strlen: O(n))*/
uint32_t ps_len(char *s) {
    uint32_t *lenptr = (uint32_t*)(s-4);
    return *lenptr;
}


char *global_string;

int main() {
    char *mystr = ps_create("hello world", 11);
    
    global_string = mystr;
    ps_retain(mystr);

    ps_print(mystr);
    printf("%s %d\n", mystr, (int)ps_len(mystr)); 
    ps_release(mystr);
    
    printf("%s\n", global_string);
    ps_release(mystr);
    return 0;
}

/* Modern allocators avoid corrupting memory if freed uncorrectly, returning an error
   (e.g. if the same pointer is freed twice). */