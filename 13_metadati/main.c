#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

    char *s = malloc(8+len+1); //4 byte for length, 1 for the terminator char
    uint32_t *lenptr = (uint32_t*)s;
    uint32_t *refcount = (uint32_t*)(s+4);
    *lenptr = len;
    
    /* Returns the string skipping length bytes.*/

    s += 8; //Skip the length and count
    for(int j = 0; j < len; j++) {
        s[j] = init[j]; //Should use memcopy() here.
    }
    s[len] = 0; //Add terminator char.
    return s;
}

/*  Display the string 's' on the screen. */
void ps_print(char *s) {
    uint32_t *lenptr = (uint32_t*)(s-4);

    for(int j=0; j < *lenptr; j++) {
        putchar(s[j]);
    }
    printf("\n");
}

/* Free a previously created PS string. */
void ps_free(char *s) {
    free(s-4);
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
    
    ps_print(mystr);
    printf("%s %d\n", mystr, (int)ps_len(mystr)); 
    ps_free(mystr);
    
    prtinf("%s\n", global_string);
    return 0;
}

/* Modern allocators avoid corrupting memory if freed uncorrectly, returning an error
   (e.g. if the same pointer is freed twice). */

// Continues in lesson 16.