/* 
    TAC command implementation (CAT inverse)

        Load file lines in a struct in order to go through them in reverse order.

        Main can receive a number (of strings) and strings as arguments (pointer to char pointer).

        For file oppening argc = 2: program_name, file_name
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct line {
    char *s;
    struct line *next; // Pointer to the struct of the following line
};

// Irrelevant where the strings are allocated, since they are all linked. They could be wherever within the computer.
// and we can still consider them as a unique structure.

// O(n) since it only looks for the location of the following line struct

int main(int argc, char **argv) { // **argv: pointer to an Array of char pointers.
    
    if (argc != 2) {
        printf("Error: Missing file name\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");      // FILE ca nbe used as a typedef. FILE is a struct file of some kind.
    // fopen(char_path, opening_mode)

    if (fp == NULL) {
        printf("Error: File does not exist\n");
        return 1;
    }

    // fgtes: system call to read fil lines. stream of characters. Retrun NULL of pointer to buffer.
    // NULL when the file ends.

    char buf[1024]; // UNSAFE if the lines are longer than 1024 chars
    struct line *head = NULL; //Pointer to the head line.

    // LINKED LIST

    while(fgets(buf, sizeof(buf), fp) != NULL) { //This fills the buffer with the stream of chars (file line)    
        
        // printf("%s", buf); // No need of \n since its read from the stream
        
        struct line *l = malloc(sizeof(struct line));
        size_t linelen = strlen(buf);
        l->s = malloc(linelen + 1);

        for (size_t j = 0; j <= linelen; j++) {
            l->s[j] = buf[j];
        }

        l->next = head; // Assign the next pointer (following line to the previous line, first iteration will be NULL)
        head = l; //THe head becomes the last node
    } 

    fclose(fp); 

    while (head != NULL) {
        printf("%s", head->s);
        head = head -> next;
    }

    return(0);
}