/**
 * FUNCTIONS WITH A VARIABLE NUMBER OF PARAMETERS (VAR ARGS)
 * (e.g.) printf with multiple fields
 * 
 * 
 * C syntax define a variadic function ... with fixed
 * arguments or variadic arguments. (Only in the signature).
 * 
 * fmt: format string, defines the order of the types
 * 
 * The actual ipmlementation uses the following functions:
 * - va_start: initializes ap that needs to be passed to va_arg
 *      (last: identifies the last non-variadic argument)
 * - va_arg: extracts variadic arguments from the list.
 * 
 *  It requires the type to avoid warning during variable assignment,
 *  it also deos not know which arguments are going to be passed.
 *  
 *  The type is therefore extracted based on the expected size.
 *      
 * - va_copy
 * - va_end
 * The C function needs to know the format of the variadic arguments
 * 
 * Each va_start call requires a call to va_end.
 * 
 * va_end must be called both when the execution is correct or 
 * with errors: GOTO is suited fo this case instead of return.
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

void foo(char *fmt, ...) {
    va_list ap;  // States of the arguments
    va_start(ap, fmt);

    char *p = fmt;
    while(*p) {
        if (*p == 'i') {
            int i = va_arg(ap, int); // Returns the integer from the variadic list
            printf("%d\n", i);
        } else if (*p == 's') {
            char *s = va_arg(ap, char*);
            printf("%s\n", s);
        } else {
            printf("Wrong specifier\n");
            goto cleanup;
        }
        p++;
    }
cleanup:
    va_end(ap);
}

/**
 * Nested variadic functions call:
 * --------------------------------------------------------------
 * Useful in the context of error management and message printing.
 * 
 * In this cases the use of vprintf can help.
 * 
 * - vsnprintf(str, buffer_size): writes in a buffer 
 * 
 * 
 * This call wrapper, 
 */

void foo2(const char *fmt, ...) {
    va_list ap;  // States of the arguments
    va_start(ap, fmt);

    char mybuf[42];
    // Read to buffer
    vsnprintf(mybuf, sizeof(mybuf), fmt, ap);
    
    // Convert the letter pf the buffer to lowercase
    for(int j=0; j < sizeof(mybuf); j++) {
        mybuf[j] = tolower(mybuf[j]);
    }
    
    // With 80 being the terminal size, pad the output to the center
    size_t len = strlen(mybuf);
    size_t padding = (80 - len)/2;

    for (size_t j = 0; j < padding; j++) {
        printf(" ");
    }
    
    // Print buffer
    printf("%s\n", mybuf);
    va_end(ap);
}

/**
 * VARIADIC FUNCTIONS allows C to work at an higher language.
 * 
 * This can allow C to be used as a query language.
 * (INstead of using sqlite_c function calls.)
 * 
 * JSON or SQLite queries, provided as strings, can use substitution
 * of characters changing the C API. l
 */

int main() {
    foo("sii", "first_printf:", 5, 10);
    foo2("%d %d\n", 5, 10);
    return 0;
}