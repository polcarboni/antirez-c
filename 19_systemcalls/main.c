/* 
   Posix:
   standardized unix system calls. Allows same system calls for several OS.
   Reduce the need for system specific calls.
   
   Some APIs are not standardized: audio, multiplexing.
*/

/* Standard libc contains some wrapper to system calls:
    Since the file is not located within the program memory space, the fopen needs to read outside of it.
    This calls an OS service that returns an ID for the file within the process.

    We rewrite some functions: open, in unix term.

    System calls:
        man 2 open 

    Open function accepts different flags depending on the way to open the file.

    SysCalls return -1 as error message.

*/

#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

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
    int fd = open("fileold.c", O_RDONLY);
    
    printf("Error number is %d\n", errno);   //errno: number referring to the error type on the manual.
    if(fd == -1) {
        perror("UNABLE TO OPEN FILE");  //perror knows the type of error
        return 1;
    }

    /* read: system call for file opening */
    char buf[32];
    ssize_t nread; //signed since read can return -1 for error
    while(1) {
        nread = read(fd, buf, sizeof(buf));
        if (nread == -1) {
            perror("Reading file:");
            return 0;
        }
        if (nread == 0) break;
        hexdump(buf, nread);
    }
     
    close(fd);  //Closing file: deallocate kernel structure dedicated to store file ref.
    return 0;
}

/* write: sysCall for writing. Can specify to write it to terminal
   
Homework: read manpage from linux and posix
   - open
   - read
   - write  
   
*/