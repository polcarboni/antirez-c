#include <stdio.h>
#include <stdlib.h>

/*
    Defining arrays for string outside the function is not functional.
    The definition of them within a 'ps_create' function will be local and wont' print.

    malloc(size_t size) is used for the allocation of requested memory, returns a void pointer (can be assigned to any type).
    free(void *ptr) removes the previously allocated memory.
*/


/*
    Initialize a prefixed length string with the specified string in 'init'
    of length 'len'.

    The created strings have the following layout:

    +-+---------------\\\
    |L|my string here!...
    +-+---------------\\\

    Where L is one unsigned byte stating the total length of the string.
    Thus this strings are binary safe: zero bytes are permitted in the middle.
    
    
    Warning: this function does not cjeck for buffer overflows.
*/

char *ps_create(char *init, int len) {
    char *s = malloc(1+len+1); //1 mor byte for the length byte, 1 for the terminator char
    unsigned char *lenptr = (unsigned char*)s;
    *lenptr = len;

    for(int j = 0; j < len; j++) {
        s[j+1] = init[j];
    }
    s[len+1] = 0; // Terminator 0
    return s;
}

/*
    Display the string 's' on the screen.
*/
void ps_print(char *s) {
    unsigned char *lenptr = (unsigned char*)s;

    for(int j=0; j < *lenptr; j++) {
        putchar(s[j+1]);
    }
    printf("\n");
}

/* Return the pointer to the null terminated C string embedded
   inside our PS string. */

char *ps_getc(char *s) {
    return s + 1;
}

int main() {
    char *mystr = ps_create("hello world", 11);
    ps_print(mystr);
    printf("%s\n", ps_getc(mystr)); //does not work because the first char is a vertical tab, while the last one night not be 0
    free(mystr);
    return 0;
}

/* When the process termiante the allocated memory is automatically freed. */