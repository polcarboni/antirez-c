
/*
    Sintassi e costrutti ancora originali:
    Compilatore e assunzioni sulla macchina si sono evolute

    GCC è stato il primo copmilatore largamente utilizzato nel free software
    
    *   hexdump -C nome_file:    per vedere il contenuto dell'eseguibile
    *   gcc -S nome_file.c:  produce il file assembly
    
    
    Perchè considerare il codice assembly, prima del codice assembly?
    Possono essere definiti con differenti tipi di ottimizzazione

    *   gcc -O2: compilazione con ottimizzazione di livello O2

    Comportamento e dimensione dell'eseguibile sono gli stessi.
    La differenza si può vedere nel file assembly, che utilizza puts e non printf. (più veloce, rimuove anche il new line).

*/

#include <stdio.h>

/*
    Direttiva al preprocessore: il compilatore chiama un programma che esegue queste direttive
    Esegue trasformazioni di testo nel sorgente del compilatore
    Il file include tutte le intestazioni della libreria stdio. <stdio.h>: header file

    Questo contiene la funzione di libreria printf.

    Si può includere anche un file c.
*/

int printf(const char *restrict format, ...);

/*
    Prototipo di una funzione, definisce il tipo di ritorno e gli argomenti.
    Anche se un programma chiama questa funzione e non conosce l'implementazione
    conosce almeno il tipo e gli input, permettendo la compilazione anche in assenza di include.
*/

int main(void) {
    printf("Hello World\n");
    return 0;
}