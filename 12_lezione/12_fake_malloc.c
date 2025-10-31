#include <stdio.h>

/* 
    Previous lesson Pascal style strings: 1st char: size.
    LHello World: 11 72 .. .. .. (No termination char, prefixed length).

    Implementazione di pls (PREFIXED LENGTH STRING).

    Due versioni: primo percorso limitato.
*/

/*
    * Initialize a prefixed length string with the specified string in 'init'
    * of length 'len'
    *
    * Warning: this function does not cjeck for buffer overflows.
*/
void ps_init(char *s, char *init, int len) {
    
    s[0] = len; //since type is int, if len>255 will be truncated
    
    unsigned char *lenptr = (unsigned char*)s;  //same thing but more correct
    *lenptr = len;              //copied the first char as string length

    for(int j=0; j<len; j++) {
        s[j+1] = init[j];
    }
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

int main() {
    char buf[256];

    //string must admit 00 term in the middle (not only as a terminator).
    
    ps_init(buf,"Hell\00 world", 11);
    ps_print(buf);
    
    return 0;
}

/*
    The need for the predefinition of string length makes it difficult to manage
    the defintion of required memory size.
*/