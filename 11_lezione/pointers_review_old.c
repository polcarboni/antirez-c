#include <stdio.h>

void printstr(char *p) {

    while (*p != '.') {
        putchar(p[0]);
        p++;
    }
    
    printf("\n");
}

int main() {

    char mystr[] = "AABBCCDDEE.FF";
    char *p = mystr;

    printf("At the beginning p is %lu\n", (unsigned long)p);
    printstr(p);
    printstr(mystr);
    printstr(mystr+3);
    printf("At the end p is %lu\n", (unsigned long)p);

    return 0;
}