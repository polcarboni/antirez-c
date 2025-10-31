#include <stdio.h>
#include <ctype.h>

#define HEXDUMP_CHARS_PER_LINE 16
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

    /* fopen allocates a file type structure and return the pointer to file.
       Return NULL if file does not exist. */
    FILE *fp = fopen("main.c", "r");

    if (fp == NULL) {
        printf("UNABLE TO OPEN FILE\n");
        return 1;
    }

    /* fread: specify object dimension and the number of items. */
    char buf[32];
    size_t nread;
    while(1) {
        nread = fread(buf, 1, sizeof(buf), fp); // Read only one object of the size of buffer
        if (nread == 0) break;
        hexdump(buf, sizeof(buf));
    }
    /* fclose closes file descriptor and avoid memory leak */
    fclose(fp);

}

/* stdio executes system calls. We could call and use these to bypass the stdlib
   for reading and printing file.  */