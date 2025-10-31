/* Typedef */

#include <stdio.h>

/* Definition of errorcode as an int alias
In thi case can be used as documentation for simple types */
typedef int errorcode;

/* Name at the end defines a single sruct. 
   The name at the top defines the name of the struct type. */
typedef struct {
    int n;
    int d;
} fract;

/* Pointer to fract type */
typedef fract *fractptr;

int foo() {
    return -20;
};

int main() {
    fract f;
    fractptr fp = &f; // No * beacause it is already a pointer
    f.n = 5;
    f.d = 7;
    printf("%d/%d Fraction stored at position %p\n", f.n, f.d, fp);
}