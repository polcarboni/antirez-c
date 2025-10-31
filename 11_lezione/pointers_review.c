#include <stdio.h>

int main() {

    char str[] = "aabb";
    short *s = (short*)str;

    // Accessing with str: single chars with str[0, 1, ....]
    // Accessing with s: first 2 bytes

    printf("%d %d\n", s[0], s[1]); //24929 25186
    

    char str2[] = {1, 1, 2, 2};
    short *s2 = (short*)str2;
    printf("%d %d\n", s2[0], s2[1]); //257 514

    /*
        ....
    */

    return 0;

}