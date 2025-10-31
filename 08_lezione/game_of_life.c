/*
    Scomposizione del problema in funzione.
    
    La prima parte del programma specializza il linguaggio portandolo a livello più alto.
*/

#include <stdio.h>
#include <unistd.h>

//define makes preprocessor substitute the words with the values

#define GRID_ROWS 20
#define GRID_COLS 70
#define GRID_CELLS (GRID_COLS*GRID_ROWS) //Le parantesi evitano problemi nell'ordine delle operazioni una volta scompattate
#define ALIVE '#'
#define DEAD '.'

/*
    Translates the x,y grid point into the index in the linear array.
    
    WRAPPING:
        Negative and positive coordinates out of the grid will be wrapped around.
*/

int cell_to_index(int x, int y) {

    if (x < 0) {
        x = (-x) % GRID_COLS;
        x = GRID_COLS - x;
    }

    if (y < 0) {
        y = (-y) % GRID_ROWS;
        y = GRID_ROWS - y;
    }

    if (x >= GRID_COLS) x = x % GRID_COLS;
    if (y >= GRID_ROWS) y = y % GRID_ROWS;
    
    //if the index is negative, resolve it making it positive.
    return y*GRID_COLS + x;
}


/* Function sets the specified cell at x,y to the specified state */

void set_cell(char *grid, int x, int y, char state) {
    //grid è un array con indice incrementale, ma l'input è passato come coordinate
    grid[cell_to_index(x,y)] = state;

    //negative values for starting from the other side
}


/* The function returns the state of the x,y cell of the grid */

char get_cell(char *grid, int x, int y) {
    return grid[cell_to_index(x,y)];
}


/*Show grid on the screen, clearing the terminal using the required VT100 escape sequence*/

void print_grid(char *grid) {
    printf("\x1b[H\x1b[2J\x1b[3J");      //clear | hexdump -C: find the thing for clearing the terminal before printing (previous lessons)
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            printf("%c", get_cell(grid,x,y));
        }
        printf("\n");
    }
}


/* Set all the grid cells the specified state */

void set_grid(char *grid, char state) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            set_cell(grid,x,y,state);
        }
    }
}


/* Return the number of living neighbors of x,y */

int count_living_neighbors(char *grid, int x, int y) {
    
    int alive = 0;

    for (int yo = -1; yo <= 1; yo++) {
        for (int xo = -1; xo <= 1; xo++) {
            if(xo == 0 && yo == 0) continue; //Do not consider the current cell, only the neighbors
            if (get_cell(grid,x+xo,y+yo) == ALIVE) alive++;
        }
    }

    return alive;    
}


/* Compute the new state of the game of life */

void compute_new_state(char *old, char*new) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            int n_alive = count_living_neighbors(old,x,y);
            int new_state = DEAD;
            if (get_cell(old,x,y) == ALIVE) {
                if (n_alive == 2 || n_alive == 3) new_state = ALIVE;
            } else {
                if (n_alive == 3) new_state = ALIVE;
            }
            set_cell(new, x, y, new_state);
        }
       
    }
}


int main() {
    /*Array per la definizione dei pixel, definite da caratteri. */
    
    char old_grid[GRID_CELLS];
    char new_grid[GRID_CELLS];

    set_grid(old_grid, DEAD);
    set_cell(old_grid, 10, 10, ALIVE);
    set_cell(old_grid, 9, 10, ALIVE);
    set_cell(old_grid, 11, 10, ALIVE);
    set_cell(old_grid, 11, 9, ALIVE);
    set_cell(old_grid, 10, 8, ALIVE);

    set_cell(old_grid, 10, 20, ALIVE);
    set_cell(old_grid, 9, 20, ALIVE);
    set_cell(old_grid, 11, 20, ALIVE);
    set_cell(old_grid, 11, 19, ALIVE);
    set_cell(old_grid, 10, 18, ALIVE);

    set_cell(old_grid, 16,12, ALIVE);
    set_cell(old_grid, 16,13, ALIVE);
    set_cell(old_grid, 16,14, ALIVE);

    set_cell(old_grid, 29,2, ALIVE);
    set_cell(old_grid, 29,3, ALIVE);
    set_cell(old_grid, 29,4, ALIVE);

    set_cell(old_grid, 7,2, ALIVE);
    set_cell(old_grid, 7,3, ALIVE);
    set_cell(old_grid, 7,4, ALIVE);

    set_cell(old_grid, 18,5, ALIVE);
    set_cell(old_grid, 18,6, ALIVE);
    set_cell(old_grid, 18,7, ALIVE);

    set_cell(old_grid, 25,25, ALIVE);
    set_cell(old_grid, 25,27, ALIVE);
    set_cell(old_grid, 24,26, ALIVE);
    set_cell(old_grid, 26,26, ALIVE);
    set_cell(old_grid, 26,27, ALIVE);

    
    while(1) {
        compute_new_state(old_grid, new_grid);
        print_grid(new_grid);
        usleep(100000);
        compute_new_state(new_grid, old_grid);
        print_grid(old_grid);
        usleep(100000);
    }
    
    return 0;
}