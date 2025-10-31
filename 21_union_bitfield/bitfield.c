#include <stdio.h>
#include <string.h>
#include <limits.h>

/* size_t of the struct is 4. */

/*  Offset of bitfield registration varies depending on LE/BE.
    An integer bitfield requires the use of the whole int.

    If a type does not use the whole int size, there is no warranties of using an adapted
    alignment ... (?).

    Vastly used for data exchange (communication protocols) when there is certainty of a conform definition,
    but now most machines are little endian.
*/


struct foo {
    unsigned char a:4;
    unsigned char b:4;
    unsigned char c:8;
};

int main() {
    struct foo f;
    // printf("%zu\n", sizeof(f));
      
    // The overflow of an unsigned char results in a wraparound
    // If signed, the operation is undefined.
    f.a = 17;
    f.b = 2;
    f.c = 17;

    printf("%d %d %d\n", f.a, f.b, f.c);
    return 0;
}