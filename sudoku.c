/*
 * sudoku.c
 *
 *  Created on: 2016年8月9日
 *      Author: WOOD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


typedef unsigned char Sv;
typedef struct {
    unsigned short f;
    Sv c;              /* row, column, square */
    Sv v;              /* cell value*/
} Sf;

struct sudoku {
    Sv cnt;
    Sv b[81];     /* board */
    Sf rf[9];     /* row flag */
    Sf cf[9];     /* column flag */
    Sf sf[9];     /* square flag */


};

static Sv sudoku_flag_index[0x200];

Sf sudoku_check_row(struct sudoku *s, int row) {
    int i = 0;
    Sf f = {0x1ff, 9, 0};
    Sv v = 0;

    for (i = 0; i < 9; i++) {
        v = s->b[row *9 + i];
        if (v != 0) {
            f.f = f.f & (~(1 << (v - 1)));
            f.c--;
        }
    }

    return f;
}

Sf sudoku_check_column(struct sudoku *s, int column) {
    int i = 0;
    Sf f = {0x1ff, 9, 0};
    Sv v = 0;

    for (i = 0; i < 9; i++) {
        v = s->b[i *9 + column];
        if (v != 0) {
            f.f = f.f & (~(1 << (v - 1)));
            f.c--;
        }
    }

    return f;
}

Sf sudoku_check_square(struct sudoku *s, int row, int column) {
    int i = 0, j = 0;
    Sf f = {0x1ff, 9, 0};
    Sv v = 0;

    row = row * 3;
    column = column * 3;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            v = s->b[(row + i) * 9 + column + j];
            if (v != 0) {
                f.f = f.f & (~(1 << (v - 1)));
                f.c--;
            }
        }
    }

    return f;
}

void sudoku_init(struct sudoku *s, FILE * in) {
    assert(in != NULL);
    int cnt = 0, c = 0, i = 0, j = 0;

    while(cnt < 81) {
        c = fgetc(in);
        if (c == EOF){
            fprintf(stderr, "Invalid file.\n");
            fclose(in);
            abort();
        }
        if (c >= '0' && c <= '9') {
            s->b[cnt++] = c - '0';
        }
    }
    fclose(in);

    s->cnt = 0;

    for (i = 0; i < 9; i++) {
        s->rf[i] = sudoku_check_row(s, i);
        s->cf[i] = sudoku_check_column(s, i);
        s->cnt = s->cnt + s->rf[i].c;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            s->sf[i * 3 + j] = sudoku_check_square(s, i, j);
        }
    }

    memset((void *)sudoku_flag_index, 0, sizeof(sudoku_flag_index));
    sudoku_flag_index[1] = 1;
    sudoku_flag_index[2] = 2;
    sudoku_flag_index[4] = 3;
    sudoku_flag_index[8] = 4;
    sudoku_flag_index[16] = 5;
    sudoku_flag_index[32] = 6;
    sudoku_flag_index[64] = 7;
    sudoku_flag_index[128] = 8;
    sudoku_flag_index[256] = 9;
}

void sudoku_print(struct sudoku *s) {
    int i = 0, j = 0;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            fprintf(stderr, "%u ", s->b[i * 9 + j]);
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


void sudoku_set_cell(struct sudoku *s, int row, int column, Sv v) {
    assert(s->b[row * 9 + column] == 0);
    s->b[row * 9 + column] = v;
    s->cnt--;

    s->rf[row].f = s->rf[row].f & (~(1 << (v - 1)));
    s->rf[row].c--;

    s->cf[column].f = s->cf[column].f & (~(1 << (v - 1)));
    s->cf[column].c--;

    s->sf[(row / 3) * 3 + (column / 3)].f = s->sf[(row / 3) * 3 + (column / 3)].f & (~(1 << (v - 1)));
    s->sf[(row / 3) * 3 + (column / 3)].c--;
}

Sf sudoku_get_cell_flag(struct sudoku *s, int row, int column) {
    assert(s->b[row * 9 + column] == 0);
    Sf f = {0x1ff, 0, 0};

    f.f = f.f & s->rf[row].f & s->cf[column].f & s->sf[(row / 3) * 3 + (column / 3)].f;

    if (sudoku_flag_index[f.f] != 0) {
        f.v = sudoku_flag_index[f.f];
        sudoku_set_cell(s, row, column, f.v);
    }

    return f;
}



int sudoku_check_row_plus(struct sudoku *s, int row, int column, Sv v) {
    assert(s->b[row * 9 + column] == 0);
    unsigned short f = 1 << (v - 1);
    int i = 0;
    Sf c_f;

    for (i = 0; i < 9; i++) {
        if (i == column) continue;
        if (v == s->b[row * 9 + i]) return 0;
        if (s->b[row * 9 + i]) continue;

        c_f = sudoku_get_cell_flag(s, row, i);
        if (c_f.f == 0) return -1;
        if (c_f.v != 0 && c_f.v != v) continue;

        if ((f & c_f.f) || (c_f.v == v)) return 0;
    }

    sudoku_set_cell(s, row, column, v);

    return 1;
}


int sudoku_check_column_plus(struct sudoku *s, int row, int column, Sv v) {
    assert(s->b[row * 9 + column] == 0);
    unsigned short f = 1 << (v - 1);
    int i = 0;
    Sf c_f;

    for (i = 0; i < 9; i++) {
        if (i == row) continue;
        if (v == s->b[i * 9 + column]) return 0;
        if (s->b[i * 9 + column]) continue;

        c_f = sudoku_get_cell_flag(s, i, column);
        if (c_f.f == 0) return -1;
        if (c_f.v != 0 && c_f.v != v) continue;

        if ((f & c_f.f) || (c_f.v == v)) return 0;
    }

    sudoku_set_cell(s, row, column, v);

    return 1;
}

int sudoku_check_square_plus(struct sudoku *s, int row, int column, Sv v) {
    assert(s->b[row * 9 + column] == 0);
    unsigned short f = 1 << (v - 1);
    int i = 0, j = 0;
    Sf c_f;

    int r = row / 3;
    int c = column / 3;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (r + i == row && c + j == column) continue;
            if (v == s->b[(r + i) * 9 + (c + j)]) return 0;
            if (s->b[(r + i) * 9 + (c + j)]) continue;

            c_f = sudoku_get_cell_flag(s, r + i, c + j);
            if (c_f.f == 0) return -1;
            if (c_f.v != 0 && c_f.v != v) continue;

            if ((f & c_f.f) || (c_f.v == v)) return 0;
        }
    }

    sudoku_set_cell(s, row, column, v);

    return 1;
}

int sudoku_check_plus(struct sudoku *s, int row, int column) {
    assert(s->b[row * 9 + column] == 0);
    Sf f = sudoku_get_cell_flag(s, row, column);
    Sv n = 0;
    int ret = 0;

    /* Set up. */
    if (f.v) return 1;

    /* Invalid Sudoku. */
    if (f.f == 0) return -1;

    /* Check */
    while (f.f) {
        n++;
        if (f.f % 2 == 1) {
            ret = sudoku_check_row_plus(s, row, column, n);
            if (ret != 0) return ret;

            ret = sudoku_check_column_plus(s, row, column, n);
            if (ret != 0) return ret;

            ret = sudoku_check_column_plus(s, row, column, n);
            if (ret != 0) return ret;
        }
        f.f = f.f >> 1;
    }

    return 0;
}

int sudoku_check(struct sudoku *s) {
    int ret = 0;
    int i = 0, j = 0;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (s->b[i * 9 + j] == 0) {
                ret = sudoku_check_plus(s, i, j);

                if (0 == ret || 1 == ret) continue;

                if (-1 == ret) {
                    //fprintf(stderr, "Invalid Sudoku.\n");
                    return -1;
                }
            }
        }
    }

    /* Completed. */
    if (s->cnt == 0) return 1;

    return 0;
}


int sudoku_resolv(struct sudoku *s) {
    Sv last = 0;
    int ret = 0;
    int cnt = 1;

    (void)cnt;
    do {
        last = s->cnt;
        ret = sudoku_check(s);
        //fprintf(stderr, "Round %d:<%d>\n", cnt++, ret);
        //sudoku_print(s);
        if (-1 == ret || 1 == ret) break;
    } while(last != s->cnt);

    return ret;
}


typedef struct {
    void *s[81];
    int top;
} sudoku_stack;

struct sudoku_try {
    struct sudoku s;
    int r;
    int c;
    Sv  v;
};

#define sudoku_stack_is_empty(S) ((S)->top == -1)

#define sudoku_stack_pop(S) (S)->s[(S)->top--]

#define sudoku_stack_push(S, V) (S)->s[++(S)->top] = (V)

int get_next(struct sudoku_try *try) {
    int i = 0, j = 0;
    Sf f;

    for (i = try->r; i < 9; i++) {
        for (j = try->c; j < 9; j++) {
            if (0 == try->s.b[i * 9 + j]) {
                f = sudoku_get_cell_flag(&(try->s), i, j);
                f.f = f.f >> try->v;
                while (f.f) {
                    try->v++;
                    if (f.f % 2 == 1) {
                        try->r = i;
                        try->c = j;
                        return 1;
                    }
                    f.f = f.f >> 1;
                }
            }
        }
    }

    return 0;
}

void sudoku_resolver(struct sudoku *s){
    int ret = 0;
    sudoku_stack *stack = NULL;
    struct sudoku_try *tmp = NULL;
    struct sudoku t;


    ret = sudoku_resolv(s);
    if (1 == ret || -1 == ret) {
        fprintf(stderr, "result<%d>:\n", ret);
        sudoku_print(s);
        return;
    }

    stack = (sudoku_stack *)malloc(sizeof(sudoku_stack));
    if (NULL == stack) {
        fprintf(stderr, "malloc failed.<stack>\n");
        abort();
    }
    memset((void *)stack, 0, sizeof(sudoku_stack));
    stack->top = -1;

    tmp = (struct sudoku_try *)malloc(sizeof(struct sudoku_try));
    if (NULL == tmp) {
        fprintf(stderr, "malloc failed.<sudoku_try>\n");
        abort();
    }

    memset((void *)tmp, 0, sizeof(struct sudoku_try));
    tmp->s = *s;
    sudoku_stack_push(stack, (void *)tmp);

    do {
        tmp = sudoku_stack_pop(stack);
        if (0 == get_next(tmp)) {
            free(tmp);
            continue;
        }
        t = tmp->s;
        sudoku_set_cell(&t, tmp->r, tmp->c, tmp->v);

        //fprintf(stderr, "Test[%d] <%d, %d, %u>.\n", stack->top + 1, tmp->r, tmp->c, tmp->v);
        ret = sudoku_resolv(&t);

        if (ret == 0) {
            sudoku_stack_push(stack, (void *)tmp);
            tmp = (struct sudoku_try *)malloc(sizeof(struct sudoku_try));
            if (NULL == tmp) {
                fprintf(stderr, "malloc failed.<sudoku_try>\n");
                abort();
            }
            memset((void *)tmp, 0, sizeof(struct sudoku_try));
            tmp->s = t;
            sudoku_stack_push(stack, (void *)tmp);
        }
        else if (-1 == ret) {
            sudoku_stack_push(stack, (void *)tmp);
        }
        else {
            free(tmp);
            break;
        }

    }while(!sudoku_stack_is_empty(stack));

    fprintf(stderr, "Test completed, result<%d>:\n", ret);
    sudoku_print(&t);

    while (!sudoku_stack_is_empty(stack)){
        tmp = sudoku_stack_pop(stack);
        fprintf(stderr, "stack[%d] <%d, %d, %u>:\n", stack->top + 1, tmp->r, tmp->c, tmp->v);
        sudoku_print(&tmp->s);
        free(tmp);
    }
    free(stack);
}

int main(int argc, char *argv[]) {

    FILE *in = NULL;
    struct sudoku s;

    if (argc == 1) {
        in = stdin;
    }
    else if (argc == 2) {
        in = fopen(argv[1], "r");
        if (in == NULL) {
            fprintf(stderr, "No such file: %s\n", argv[1]);
            abort();
        }
    }
    else {
        fprintf(stderr, "Usage:%s [file name]\nif file name not given, use stdin.\n", argv[0]);
        return 0;
    }

    sudoku_init(&s, in);

    fprintf(stderr, "Init Suidoku:\n");
    sudoku_print(&s);

    sudoku_resolver(&s);

    return 0;
}
