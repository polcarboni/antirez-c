#include <stdio.h>
#include <stdlib.h>


/*
    Struct: aggregated type. Only C thing that is not about editing numbers but DATA TYPES.

    +-+----+----+
    |c|num |den |
    +-+----+----+

    Each element is registered in a volume of its size
*/

struct frac {
    unsigned char color;
    int num;
    int den;
};


// By adding a single char the size goes from 8 to 12 instead of 9. Default strcuts guarantee that
// each member is saved in memory to a register multiple of its size.
// It will add 3 padding bytes to the first char to make the int start at the correct offset.

// Also by adding the char to the end. Struct must be aligned to the biggest of its variable size.
// That happens to allow alignment of arrays of variables.

// If the chars are correctly places they can replace the paddings. (It is possible to manually avoid the definition of padding).


/* 
    Definition of fractions.
*/

void set_fraction(int *f, int num, int den) {
    
    /* 
        Write in pointed memory location the numerator and denominator of the fraction     
                |f[0]|f[1]|
                |1111|2222|
    */
    
    f[0] = num;
    f[1] = den;
}


/*  --------------------------------------------------------------------------------------
 *   Create a new fraction, setting num and den as the numerator and denumerator.
 *   The function returns NULL on out of memory, otherwise the fraction object is returned.
 *  --------------------------------------------------------------------------------------
*/  

int *create_fraction (int num, int den) {
    //This version avoids the need to manually execute malloc
    
    int *f = malloc(sizeof(int)*2); //8 byte for the fraction f
    if (f == NULL) return NULL; //malloc error checking. Usually in libraries.
    f[0] = num;
    f[1] = den;
    
    return f;
}


/*  --------------------------------------------------------------------------------------
 *   Simplify the provided fraction.
 *  --------------------------------------------------------------------------------------
*/ 
void simplify_fraction(int *f) {
    for (int d = 2; d <= f[0] && d <= f[1]; d++) {
        while (f[0] % d == 0 && f[1] % d == 0) {
            f[0] /= d;
            f[1] /= d;
        }
    }
}

void print_fraction(int *f) {

    // Print the fraction

    printf("%d/%d\n", f[0], f[1]);
}

int main() {

    printf("%d\n", (int)sizeof(struct frac)); // 8 -> 4 bytes for integer type 

    struct frac a;
    struct frac *b = &a;

    // b.num = 1;   Cannot access to referenced struct variables through the . operator
    b->den = 2;   // You must use the -> operator.

    // THe compiler can tell its a pointer but for clarity of intent of using a pointer. 

    printf("%d/%d", a.num, a.den);

    #if 0
    // Preprocessor definition for comment

    int *f = malloc(sizeof(int)*2); //8 byte for the fraction f
    set_fraction(f, 1, 2);
    
    int *f2 = create_fraction(10, 20);


    print_fraction(f);
    print_fraction(f2);
    simplify_fraction(f2);
    print_fraction(f2);
    return 0;

    #endif
}


