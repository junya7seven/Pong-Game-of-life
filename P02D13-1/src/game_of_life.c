#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define X_SIZE 80
#define Y_SIZE 25

void allocateGrid(int ***grid);
void freeGrid(int **grid);
void initializeGrid(int **grid);
void initializeGridFromFile(int **grid, FILE *file);
void printGrid(int **grid, int countGen, int speed);
int countNeighbors(int **grid, int x, int y);
void updateGrid(int **grid, int **newGrid);
int gameMenu(int *speed);
int countAliveCells(int **grid);

int main(int argc, char *argv[]) {
    int **grid;
    int **newGrid;
    int speed = 1;
    int countGen = 0;
    srand(time(NULL));

    initscr();
    noecho();

    allocateGrid(&grid);
    allocateGrid(&newGrid);

    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "n/a %s\n", argv[1]);
            return 1;
        }
        initializeGridFromFile(grid, file);
        fclose(file);
    } else {
        initializeGrid(grid);
    }

    while (1) {
        erase();
        printGrid(grid, countGen, speed);

        updateGrid(grid, newGrid);

        countGen++;

        halfdelay(speed);

        if (gameMenu(&speed) == -1) break;
    }

    endwin();

    freeGrid(grid);
    freeGrid(newGrid);

    return 0;
}

void allocateGrid(int ***grid) {
    *grid = (int **)malloc(Y_SIZE * sizeof(int *));
    for (int y = 0; y < Y_SIZE; y++) {
        (*grid)[y] = (int *)malloc(X_SIZE * sizeof(int));
    }
}

void freeGrid(int **grid) {
    for (int y = 0; y < Y_SIZE; y++) {
        free(grid[y]);
    }
    free(grid);
}

void initializeGridFromFile(int **grid, FILE *file) {
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            fscanf(file, "%d", &grid[y][x]);
        }
    }
}

int gameMenu(int *speed) {
    char choice = getch();

    switch (choice) {
        case '1':
            *speed = (*speed > 1) ? *speed - 1 : 1;
            return 0;
        case '2':
            *speed += 1;
            return 0;
        case 'q':
            return -1;
        default:
            break;
    }
    return 0;
}

void initializeGrid(int **grid) {
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            grid[y][x] = rand() % 2;
        }
    }
}

void printGrid(int **grid, int countGen, int speed) {
    int aliveCells = countAliveCells(grid);
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            if (grid[y][x] == 1) {
                printw("0");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
    printw("=GEN: %d Speed: %d ", countGen, speed);
    printw("Life cells: %d=", aliveCells);
}

int countAliveCells(int **grid) {
    int count = 0;
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            if (grid[y][x] == 1) {
                count++;
            }
        }
    }
    return count;
}

int countNeighbors(int **grid, int x, int y) {
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            int neighborX = (x + i + X_SIZE) % X_SIZE;
            int neighborY = (y + j + Y_SIZE) % Y_SIZE;

            count += grid[neighborY][neighborX];
        }
    }
    return count;
}

void updateGrid(int **grid, int **newGrid) {
    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            int neighbors = countNeighbors(grid, x, y);

            if (grid[y][x] == 1 && neighbors < 2) {
                newGrid[y][x] = 0;
            } else if (grid[y][x] == 1 && (neighbors == 2 || neighbors == 3)) {
                newGrid[y][x] = 1;
            } else if (grid[y][x] == 1 && neighbors > 3) {
                newGrid[y][x] = 0;
            } else if (grid[y][x] == 0 && neighbors == 3) {
                newGrid[y][x] = 1;
            } else {
                newGrid[y][x] = grid[y][x];
            }
        }
    }

    for (int y = 0; y < Y_SIZE; y++) {
        for (int x = 0; x < X_SIZE; x++) {
            grid[y][x] = newGrid[y][x];
        }
    }
}
