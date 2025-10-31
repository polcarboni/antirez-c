/*
 * Function pointer
 *
 * Pointers until now are only defined to data memory address.
 * Functions are however also stored in stack memory addresses.
 * 
 * Allows calling a function several times.
 *
 * e.g. qsort stdlib function gets a comparator function
 */

#include <stdio.h>

void hello(void) {
	printf("Hello!\n");
}

void baubau(void) {
	printf("bau bau\n");
}


/*
 * Calls the pointed function n times.
 */
void call_n_time(int n, void(*x)(void)) {
	while(n--) { // Return value before decrement
		x();
	}
}


// Function name is a pointer that I can print.
int main() {

	//Function pointer: not a funct prototype
	int (*myf)(void);
	void (*x)(void);
	myf = main; // Functpointer
	
	printf("%p\n", &main);	
	printf("%p\n", myf);	

	// Infinite loop (segfault): with -O3 optimization compilation the address is always the same.
//	main();

	//Pointers can be called as regular functions
	//myf(); //Infinite loop again


	x = hello;
	x();	
	x = baubau;
	x();	

	call_n_time(10, hello);
	return 0;
}
