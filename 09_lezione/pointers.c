/*
            INTRODUZIONE AI PUNTATORI
*/

#include <stdio.h>

void incr(int *p) {
    *p = *p + 1;
}

int main() {
    int x = 5;
    int *y = NULL; //Il puntatore è riferito all'elemento successivo (non al tipo)
        //inizializzato all'indirizzo nullo/0
    
    printf("x was %d\n", x);
    
    y = &x; //Indirizzo di memoria in cui la variabile x è localizzata, assegnato ad y

    // Il tipo di puntatore è utile nonstante non sia riferito ad una variabile
    // ma ad un indirizzo di memoria

    printf("x stored at %p\n", y);

    *y = 10; //All'indirizzo di memoria y, in cui è definito precedentemente un intero, scrivi 10.
    y[0] = 10;  //same, non può andare oltre 0 perchè punta ad un singolo intero

    printf("x now is %d\n\n", x);
    
    /*
        Why do not edit x directly?
    */
    
    int a = 5;
    int *b = NULL;

    printf("a was %d\n", a);
    printf("%p\n", &a);
    incr(&a);

    printf("a is now %d\n", a);

    //Il valore aumenta senza che venga modificato x

    return 0;
}