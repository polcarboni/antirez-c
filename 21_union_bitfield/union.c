#include <stdio.h>
#include <string.h>
#include <limits.h>

/*  Features: Union, Bitfield, function pointer (next time).
    
    Union: make fields start at the same offset. Memory address share.
           anonymous union available from C99.

           Is common to use unions without needing to specify.
           When creating a struct that can have several types, use this to avoid allocating memory for all the types without actual need.
*/

/* IIIIAAAA */

struct foo {
    /* Fields start at the same offset */
    union {
        int i;
        unsigned char a[4];
    };
};


int main() {
    struct foo f;
    f.i = INT_MAX;
    // memcpy(f.a, "abcd", 4);

    /* The first element (least significative) of the array is set as the i member */
    printf("%d [%d %d %d %d]\n", f.i, f.a[0], f.a[1], f.a[2], f.a[3]);
}