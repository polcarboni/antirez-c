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

/*  --------------------------------------------------------------------------------------
 *   Create a new fraction, setting num and den as the numerator and denumerator.
 *   The function returns NULL on out of memory, otherwise the fraction object is returned.
 *  --------------------------------------------------------------------------------------
*/  

struct frac create_fraction (int num, int den) {

    //This version avoids the need to manually execute malloc
    

    // Struct can be returned by functions since its a data type.
    struct frac f; // Crate the fraction as a local variable and return it.

    // The function type is struct frac and not a pointer anymore.

    f.num = num;
    f.den = den;
    
    return f;
}


/*  --------------------------------------------------------------------------------------
 *   Simplify the provided fraction.
 *  --------------------------------------------------------------------------------------
*/ 
void simplify_fraction(struct frac *f) {
    for (int d = 2; d <= f->num && d <= f->den; d++) {
        while (f->num % d == 0 && f->den % d == 0) {
            f->num /= d;
            f->den /= d;
        }
    }
}

void print_fraction(struct frac *f) {
    printf("%d/%d\n", f->num, f->den); // Avoids the need to know the meaning of the index
}

int main() {

    struct frac f1 = create_fraction(10,20);
    struct frac f2 = create_fraction(3,4);


    // Add the dereference since the functions takes a pointer to the fraction. 
    
    simplify_fraction(&f1);
    print_fraction(&f1);
    print_fraction(&f2);


    /* 
        First 2 functions are assigned to and return a structure.

        This makes the program IMPLICIT COPY the structure created in the create_fraction function assigning it to the one
        defined in the main. (With the pointer there is no implicit copy).

        This is an inefficiency of the program.
        It is bad practice to return structure by copy and not by pointer (get worse for bigger structures).

    */

    return 0;
}


