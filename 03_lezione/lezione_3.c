#include <stdio.h>



int x = 0;

/*
    La variabile di x è locale
*/

int incr_local() {
    int x = 1;
    x = x + 1;
    printf("%d\n", x);
}


int incr() {
    x = x + 1; // Non trovando x, riconosce la definizione globale.
    printf("%d\n", x);
}

int main() {

    int a = 10;
    float b = 1.34;
    printf("%d %f\n", a, b);
    return 0;
}