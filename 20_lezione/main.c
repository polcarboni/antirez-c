/* When to use system calls or C library? 

   libc implements buffering in the output to reduce system calls (expensive procedures): calling write for each byte would be onerous.
   syscalls are not able to read line by line (libc feature).

   fgets: library calls that read a line up to \n (retained). HIgher level read that read from file. Based on libc (uses buffer)

   libc programs can be portable.
*/

/* Scheduler: In each kernel the scheduler decides which process to execute.
    If a process is blocked (e.g. read in a TCP socket waiting for data packets from net).

    sleep: process is paused without any cycle time.
*/

/* Multitasking preemptive system: allows execution of different parallel processes. */

#include <stdio.h>
#include <unistd.h> //without this is portable to ANSI_C (?) basic C compiler. 
#include <string.h>

#include <sys/mman.h>
#include <fcntl.h>

int main2() {

    /* Calling system print() for each char. Use write for each char of the string. */
    char *mystr = "Hello world!";
    size_t len = strlen(mystr);
    
    for (size_t j=0; j<len;j++) {
        // write(STDOUT_FILENO, mystr+j, 1); //args: file_descriptor (terminal), char_pointer, 1 byte.
        
        putchar(mystr[j]);  //putchar: calls write using a buffer
        // putchar("\n");   //NOT ONLY NEW LINE: empties the putchar buffer.
        
        // sleep(1); //too slow
        usleep(50000); //milliseconds
        fflush(stdout); //write pending data to stdout.
    }
    
    printf("Hello world");
    sleep(5);

    return 0;
}

/* mmap: syscall that map the content of a file to the process memory, accessible with pointers. 
    args: addr (starting address of the map), depending on MAP_FIXED flag (it can remove possibly preexisting map (e.g. malloc())). if not used, automatic selection.
    size in byte of the map, ..., offset.
    prot: access protection (PROT_NONE, PROT_READ, PROT_WRITE, PROT_EXEC).
    boh: MAP_ANON, MAP_FILE, ...
*/

int main() {
    int fd = open("main19.c", O_RDONLY);
    printf("Open file descriptor: %d\n", fd);

    // void *mem = mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
    void *mem = mmap(NULL, 100, PROT_READ, MAP_FILE|MAP_SHARED, fd, 0);
    printf("File mapped at %p\n", mem);

    /* When error happen, mmap returns the last available address (last byte of the memory space addressed by the process.) 
    This design allows the referencgin to the 0 address.*/

    char *s = mem;
    for(int j= 0; j<50; j++) {
        printf("s[%d] = %c\n", j, s[j]);
    }


    /* mmap allows faster programs. CPU activates kernel when accessing to the referenced memory space and copies from file to memory space.
        This can happen the other way around.
        
        A single system call can write all the inputs from memory to file.
        Can simplify the implementation of some programs.
    */

    return 0;
}