#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * Array structure is the following:
 *      00001111222233334444....9999 
 *  To compare elements qsort use a callback pointing to the different element of the array.
 *  Knows the size and length of element in the array.
 */


int compare_integers(const void *a, const void *b) {
//const: the function deos not modify the pointed location
    
    const int *ap = a; //convert pointers to correct type
    const int *bp = b;
    
    if(ap[0] > bp[0]) return 1;
    else if(ap[0] < bp[0]) return -1;
    else return 0;

}


int main(void) {
    int a[10];
    for (int j=0; j<10; j++) {
        a[j] = rand() &15; //mask to 15. In binary 15 has the first 4 bits as 1.Bitwise and for only last values.
			   //Number generated with are all the same.
    }
    
    //Sorting array
    a[2] = INT_MAX; //Check if the sorting works with wraparound and does not overflow
    a[5] = INT_MIN;
    qsort(a,10,sizeof(int),compare_integers);
    
    
    for (int j=0; j<10; j++) {
        printf("%d ", a[j]);
    }
    
    printf("\n");
    return 0;
}
