#include <stdio.h>

void incr(int *p) {

    //dereferencing: accessing value through the memory index

    printf("Before incr: %d\n", *p);
    p[0] = p[0] + 1; 
    printf("After incr: %d\n", *p);
}


int main() {
    int x = 5;
    
    
    int *y = &x;    //&: estrae l'indirizzo di memoria a cui è savlata x
    int **z = &y;   //puntatore ad un puntatore

    printf("x lives in %p and values %d\n", y, x);
    printf("x is stored in %p and y is stored in %p\n", y, z);

    // Pointers are numbers, we can print their size with sizeof. (int) casting avoids printf warning.
    // All pointers are the same size (deos not depend on which kind of variable they point to)

    printf("%d, %d, %d\n\n", (int)sizeof(y), (int)sizeof(z), (int)sizeof(x));
    
    
    /*
        _________ Strings are character arrays _________
    */

    char mystr[] = "Hello World";
    char *p = mystr; //Puntatore ad un indrizzo di memoria che contiene un array.
    // Non serve & perchè il nome dell'array è il puntatore
    
    printf("At %p i saved %s\n", mystr, mystr); //Il nome può essere usato sia come puntatore che come valore
    
    // Character print: wiht p[2] notation we can access all the string characters
    // We can acheve the same by incrementing pointers (they are numbers) 
    printf("%c%c\n", p[0], p[1]);



    /*
        short: 16 bit integers
    */

    char mystr2[] = "AABBCCDDEEFF";
    short *q = (short*)mystr2;  //pointer casting: avoid the compiler warning

    printf("%d\n", *q); //16705
    
    q++;
    printf("%d\n", *q); //16962 invece che 16961

    // pointer increment is based on the type size (short is 2 bytes), skips 2 chars when incremented
    // same happens when using the array notation: q[4] goes to FF

    char *c = mystr2;

    printf("At the beginning c is %lu\n", (unsigned long)c);

    while (*c != 0) { // null term at the end of the string
        putchar(*c);  // print char
        c++;
    }
    printf("At the end c is %lu\n", (unsigned long)c);

    // The function within the while statement can be defined as a print string funtion

    return 0;
}