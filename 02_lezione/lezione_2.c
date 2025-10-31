#include <stdio.h>

/*
    a+b: espressione, accettata dal return

*/

int sum(int a, int b) {
    return a+b;
}

/*
    Funzione main: 
        * int: valore di ritorno intero
        * () or (void): argomenti di input accettati, nessuno
     
        La funzione main viene eseguita automaticamente all'inizio del programma.
*/

int main() {
    int a, b, c;
    
    a = 10;
    b = 20;
    c = a + b;
    
    printf("Hello World %d\n", c);
    return 0;
}

/*
    * printf: print formatted, stampa in maniera formattata con:
        *   arg1: formato, arg2: int.
    
    In presenza di due argomenti %d %d nel formato, e un solo intero, si ha un warning per comportamento imprevedibile.

    Il return del main serve a comunicare il funzionamento del programma al sistema operativo.

    *   ./exec.out; echo $?: stampa il return
    *   ./exec.out && ls: l'esecuzione della seconda istruzione è condizionata dal return della prima
    *                     se return diverso da 0 non esegue ls.
*/

/*
    Fondamentale dichiarare esplicitamente il tipo della variabile.

    Dichiarazione e l'assegnazione del valore alla variabile posso essere separate.

    Le variabili all'interno di sum sono __variabili_locali__:
        create alla chiamata della funzione ed esistenti solo durante l'esecuzione.
        al termine dell'esecuzione vengono rimosse insieme ai parametri
*/





//------------------------------------------- SECONDA PARTE -------------------------------------------------

// EASY 6502
// Compiler explorer: vizualizza assambly del codice (assembly 386 semplice)

/*
    Le variabili locali sono più semplici con linguaggi di alto livello (con garbage collector).

    Creazione e distruzione di una variabile locale:

    Tutti i microprocessori sono dotati di set di registri (variabili veloci all'interno del core del processore).
    Contengono solo numeri interi o float (quelli nuovi), nominati in maniera sequenziale.

    Oltre operarae su questi usano operazioni in grado di muovere dati tra RAM e registri.

    Processori CISC: utilizzo diretto di istruzioni tra locazioni di memoria senza passaggi tra i registri.

    _______Processore_6502_(Commodore64)______
    + Registro A:
    START:
        LDA 50: carica valore 50 nel registro A
        JSR FILL_FIVE: salta alla funzione
        BRK
    FILL_FIVE:
        TAX: transfer A in X (trasferisce il valore nel registro X). Se = 0 il interruzione
        BEQ
        LDY 500: carica 0 in Y
        LDA 505: carica 5 in A (entrambi sovrascrivono il valore precedente)
    LOOP:
        STA $0200, Y : (Store value A) alla locazione 200 indirizzo di partenza e offset
        INY: increment Y (store al valore successivo).
        DEX: decrementa x
        BNE LOOP: condizione, se x non è 0 ripeti il loop
    DONE:
        RTS

    I valori delle variabili in C vengono persi perhcè usano i registri.


    386 (Architettura):
        Utilizzo dello stack come convenzione per la chiamata alle funzioni.

        Stack: pezzo di memoria diviso in settori, dotato di uno stack pointer.
               Lo stack pointer indica uno di questi.
        
        Alcune istruzioni come push A:
            SP decrementato di 4 bit (dimensione) e inserisce il valore di A.
            Push B, uguale.
            
            Pop B:
                riprende il contenuto dallo stack


    Diversi compilatori devono usare lo stesso standard per il processore.


    Call: utilizza il program counter per saltare alla funzione e una volta eseguita tornare al main,
          nel main, dopo la funzione è necessario specificare l'indirizzo di memoria.
*/