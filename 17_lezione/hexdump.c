#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Struct, string design, Hexdump() */

struct pls{
    long len; // long Avoid padding on 64 bit machines
    // char *str; // This pointer exists within the struct, occupies 4 bytes.
    char str[20]; // Fixed length 20 char array. Not a member, an offset to where the chars can be allocated.
};

// Prints offset, bytes, and content of a file.
#define HEXDUMP_CHARS_PER_LINE 8
void hexdump(void *p, size_t len) {
    unsigned char *byte = p;  
    size_t po = 0;      // Already printed chars
    
    for (size_t j = 0; j < len; j++) {
        printf("%02X ", byte[j]); // Print an int number as HEX
        
        if ((j+1) % HEXDUMP_CHARS_PER_LINE == 0 || j == len-1) { // New line every 8 numbers or after printing last num
            
            if (j == len-1) { // Output alignment with padding for uneven lengths
                int pad = HEXDUMP_CHARS_PER_LINE -
                    (len % HEXDUMP_CHARS_PER_LINE);
                    pad %= HEXDUMP_CHARS_PER_LINE;
                for (int i = 0; i < pad; i++){
                    printf("~~ ");
                }
            }
            printf("\t");
            
            // Print the human readable characters of the string
            for (size_t i = po; i <= j; i++) {

                // Ternary operator: if the isprint condition is true: return the lvalue, otherwise return rvalue.
                int c = isprint(byte[i]) ? byte[i] : '.';
                printf("%c", c);
            }
            printf("\n");
            po = j+1; // Next unprinted char
        }
    }
}

int main() {
    struct pls s;
    s.len = 10;
    //s.str = malloc(10+1); // Cannot be allocated if defined as an array
    
    // Initialize the struct memory with the same value (avoid random uninitialized values)
    //memset(&s, 0xff, sizeof(s));

    // memcpy: given a dst and a src pointer, copy the content.
    memcpy(s.str, "1234567890", 11); // The provided string get casted into a pointer.

    // memcopy skips the 8 byte of the s.len offset to point s.str.
    printf("%p\n", &s);
    printf("%p\n", s.str); // These prints highlight the 8byte difference in the pointers 


    hexdump(&s, sizeof(s)-2); // Little endian, read backwards
    // 0A 00 00 00 00 00 00 00 A0 62 FE 48 32 5E 00 00 : first 8 bytes integer number.
    
    return 0;
}