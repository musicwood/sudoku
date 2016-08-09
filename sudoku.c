#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct cell {
    unsigned short v;
    unsigned short f;
} Cell;

static Cell sudoku[81], last_sudoku[81];

static const int sudoku_size = sizeof(Cell) * 81;

#if 0
unsigned char test_1[81] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};
#endif


#if 1

unsigned char test_1[81] = {
    0, 8, 6, 1, 7, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 9, 0, 0, 0, 0, 0, 8,
    7, 0, 0, 0, 0, 0, 1, 5, 0,
    0, 0, 0, 0, 0, 0, 0, 6, 0,
    0, 3, 0, 0, 0, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 9, 0, 0, 4,
    0, 2, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 6, 0, 0, 0, 0, 0
};


#else

unsigned char test_1[81] = {
    0, 6, 0, 0, 0, 2, 0, 0, 8,
    0, 0, 3, 0, 6, 0, 4, 0, 9,
    0, 0, 0, 0, 1, 0, 7, 0, 0,
    0, 0, 0, 7, 9, 0, 8, 0, 0,
    6, 0, 0, 0, 0, 0, 0, 0, 5,
    0, 0, 7, 0, 8, 6, 0, 0, 0,
    0, 0, 9, 0, 4, 0, 0, 0, 0,
    4, 0, 1, 0, 5, 0, 9, 0, 0,
    7, 0, 0, 8, 0, 0, 0, 2, 0
};


unsigned char test_1[81] = {
    8, 0, 7, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 4, 0, 7, 6, 0,
    4, 0, 0, 0, 0, 7, 0, 0, 5,
    2, 0, 6, 0, 0, 5, 0, 0, 0,
    0, 0, 5, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 8, 0, 0, 6, 0, 9,
    7, 0, 0, 1, 0, 0, 0, 0, 8,
    0, 3, 9, 0, 8, 0, 0, 0, 6,
    0, 0, 0, 0, 0, 0, 9, 0, 2
};


unsigned char test_1[81] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};



unsigned char test_1[81] = {
    0, 0, 0, 0, 4, 0, 7, 0, 0,
    0, 0, 7, 0, 0, 1, 0, 5, 0,
    0, 0, 8, 3, 0, 2, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 5, 6, 0, 3, 9, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 6,
    0, 0, 0, 9, 0, 5, 2, 0, 0,
    0, 9, 0, 4, 0, 0, 3, 0, 0,
    0, 0, 2, 0, 1, 0, 0, 0, 0
};

unsigned char test_1[81] = {
    0, 3, 0, 0, 0, 1, 6, 7, 0,
    0, 7, 9, 0, 0, 0, 0, 0, 0,
    2, 1, 8, 0, 0, 0, 0, 0, 4,
    0, 4, 0, 0, 0, 9, 3, 0, 0,
    9, 0, 0, 0, 1, 0, 0, 0, 2,
    0, 0, 1, 2, 0, 0, 0, 5, 0,
    8, 0, 0, 0, 0, 0, 7, 4, 5,
    0, 0, 0, 0, 0, 0, 8, 6, 0,
    0, 9, 5, 8, 0, 0, 0, 3, 0
};
#endif


static void sudoku_init(unsigned char in[]) {
    int i = 0;
    
    for (i = 0; i < 81; i++) {
        if (in[i] < 0 || in[i] > 9) {
            fprintf(stderr, "Error in sudoku_init, input is not a vaild sudoku array.\n");
            abort();
        }
        else if (in[i] == 0) {
            sudoku[i].v = 0;
            sudoku[i].f = 0x1ff;
        }
        else {
            sudoku[i].v = in[i];
            sudoku[i].f = 0;
        }

    }    
}

static void sudoku_print(void) {
    int i = 0, j = 0;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            fprintf(stderr, "%u ", sudoku[i * 9 + j].v);
            if (j == 2 || j == 5) {
                fprintf(stderr, "| ");
            }
        }
        fprintf(stderr, "\n");
        if (i == 2 || i == 5) {
            fprintf(stderr, "- - -   - - -   - - -\n");
        }
    }
    fprintf(stderr, "\n");
}

static unsigned short sudoku_check_row(int row) {
    int i = 0;
    unsigned short v = 0, f = 0x1ff;

    for (i = 0; i < 9; i++) {
        v = sudoku[row * 9 + i].v;
        if (v != 0) {
            f = f & (~(1 << (v - 1)));
        }
    }

    return f;
}

static unsigned short sudoku_check_column(int column) {
    int i = 0;
    unsigned short v = 0, f = 0x1ff;

    for (i = 0; i < 9; i++) {
        v = sudoku[i * 9 + column].v;
        if (v != 0) {
            f = f & (~(1 << (v - 1)));
        }
    }

    return f;

}

static unsigned short sudoku_check_big_cell(int row, int column) {
    int i = 0, j = 0;
    unsigned short v = 0, f = 0x1ff;

    row = row - (row % 3);
    column = column - (column % 3);
    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            v = sudoku[(row + i) * 9 + column + j].v;
            if (v != 0) {
                f = f & (~(1 << (v - 1)));
            }

        }
    }

    return f;
}



static void sudoku_check_fix_row(int row, unsigned short v) {
    int i = 0;
    unsigned short f = 0x1ff & (~(1 << (v - 1)));

    for (i = 0; i < 9; i ++) {
        v = sudoku[row * 9 + i].v;
        if (v == 0) {
            sudoku[row * 9 + i].f = sudoku[row * 9 + i].f & f;
        }
    }
}

static void sudoku_check_fix_column(int column, unsigned short v) {
    int i = 0;
    unsigned short f = 0x1ff & (~(1 << (v - 1)));

    for (i = 0; i < 9; i ++) {
        v = sudoku[i * 9 + column].v;
        if (v == 0) {
            sudoku[i * 9 + column].f = sudoku[i * 9 + column].f & f;
        }
    }
}

static void sudoku_check_fix_big_cell(int row, int column, unsigned short v) {
    int i = 0, j = 0;
    unsigned short f = 0x1ff & (~(1 << (v - 1)));

    row = row - (row % 3);
    column = column - (column % 3);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            v = sudoku[(row + i) * 9 + column + j].v;
            if (v == 0) {
                sudoku[(row + i) * 9 + column + j].f = sudoku[(row + i) * 9 + column + j].f & f;
            }
        }
    }
}

static void sudoku_check_fix(int row, int column, unsigned short v) {
    sudoku_check_fix_row(row, v);
    sudoku_check_fix_column(column, v);
    sudoku_check_fix_big_cell(row, column, v);
}

static void sudoku_check_flag(int row, int column) {
    unsigned short f = sudoku[row * 9 + column].f;
    unsigned short v = 0;
    int have = 0;
    assert(0 == sudoku[row * 9 + column].v);
    
    while (f) {
        v++;
        if (f % 2 == 1) {
            have = 1;
        }

        f = f >> 1;
        if (1 == have) {
            break;
        }
    }
    
    if (have == 0) {
        sudoku_print();
        fprintf(stderr, "<r:%d, c:%d> Invaild sudoku.\n", row, column);
        abort();
    }
    else if (f > 0) {
        /* Have more than one numbers can fill in this blank, do nothing. */
    }
    else {
        /* Only v can fill in this blank. */
        sudoku[row * 9 + column].v = v;
        sudoku[row * 9 + column].f = 0;
        sudoku_check_fix(row, column, v);
    }
}


static int sudoku_check_plus_row(int row, int column, unsigned short v) {
    int i = 0;
    unsigned short f = 1 << (v - 1);

    for (i = 0; i < 9; i++) {
        if (i == column) continue;
        if (0 != sudoku[row * 9 + i].v) continue;

        if (f & sudoku[row * 9 + i].f) return 0;
    }

    return 1;
}

static int sudoku_check_plus_column(int row, int column, unsigned short v) {
    int i = 0;
    unsigned short f = 1 << (v - 1);

    for (i = 0; i < 9; i++) {
        if (i == row) continue;
        if (0 != sudoku[i * 9 + column].v) continue;

        if (f & sudoku[i * 9 + column].f) return 0;
    }

    return 1;
}

static int sudoku_check_plus_big_cell(int row, int column, unsigned short v) {
    int i = 0, j = 0;
    unsigned short f = 1 << (v - 1);

    int r = row - (row % 3);
    int c = column - (column % 3);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (r + i == row && c + j == column) continue;
            if (0 != sudoku[(r + i) * 9 + c + j].v) continue;
            
            if (f & sudoku[(r + i) * 9 + c + j].f) return 0;
        }
    }

    return 1;
}

static unsigned short sudoku_check_plus(int row, int column) {
    unsigned short f = sudoku[row * 9 + column].f;
    unsigned short v = 0;

    while (f) {
        v++;
        if (f % 2 == 1) {
            if (sudoku_check_plus_row(row, column, v)) return v;
            if (sudoku_check_plus_column(row, column, v)) return v;
            if (sudoku_check_plus_big_cell(row, column, v)) return v;
        }

        f = f >> 1;
    }
    
    return 0;
}

static void sudoku_check(void) {
    int i = 0, j = 0;
    unsigned short v = 0, f = 0x1ff;
    unsigned short flag = 0;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            v = sudoku[i * 9 + j].v;
            if (v == 0) {
                flag = 0;
                f = 0x1ff;
                f = f & sudoku_check_row(i);
                f = f & sudoku_check_column(j);
                f = f & sudoku_check_big_cell(i, j);
                sudoku[i * 9 + j].f = f;
                sudoku_check_flag(i, j);

                if (sudoku[i * 9 + j].v == 0) {
                    flag = sudoku_check_plus(i, j);
                    if (flag) {
                        sudoku[i * 9 + j].v = flag;
                        sudoku[i * 9 + j].f = 0;
                        sudoku_check_fix(i, j, flag);
                    }
                }
            }
        }
    }
}


int main(void) {
    int round = 1;

    sudoku_init(test_1);
    fprintf(stderr, "Init sudoku:\n");
    sudoku_print();

again:
    memcpy((void *)last_sudoku, (void *)sudoku, sudoku_size);

    sudoku_check();
    fprintf(stderr, "Round %d:\n", round++);
    sudoku_print();
    
    if (0 != memcmp((void *)last_sudoku, (void *)sudoku, sudoku_size)) {
        goto again;
    }
    
    return 0;    
}
