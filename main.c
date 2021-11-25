#include <stdio.h>

//przyjmując: 0-król, 1-hetman, 2-wieża, 3-goniec, 4-skoczek,
//5-pionek, 6-król_k, 7-hetman_k, 8-wieża_k, 9-goniec_k,
//10-skoczek_k, 11-pionek_k, 12-pole puste

int MAX_DIRECTION[]={8, 8, 4, 4, 8, 3, 8, 8, 4, 4, 8, 3, 0};
int MAX_DISTANCE[]={2, 8, 8, 8, 2, 2, 2, 8, 8, 8, 2, 2, 0};

int VEC_X[12][8]={{0, 1, 1, 1, 0, -1, -1, -1}, {0, 1, 1, 1, 0, -1, -1, -1}, {0, 1, 0, -1}, {1, 1, -1, -1}, {1, 2, 2, 1, -1, -2, -2, -1}, {-1, 0, 1}, {0, 1, 1, 1, 0, -1, -1, -1}, {0, 1, 1, 1, 0, -1, -1, -1}, {0, 1, 0, -1}, {1, 1, -1, -1}, {1, 2, 2, 1, -1, -2, -2, -1}, {-1, 0, 1} };
int VEC_Y[12][8]={{-1, -1, 0, 1, 1, 1, 0, -1}, {-1, -1, 0, 1, 1, 1, 0, -1}, {-1, 0, 1, 0}, {-1, 1, 1, -1}, {-2, -1, 1, 2, 2, 1, -1, -2}, {-1, -1, -1}, {-1, -1, 0, 1, 1, 1, 0, -1}, {-1, -1, 0, 1, 1, 1, 0, -1}, {-1, 0, 1, 0}, {-1, 1, 1, -1}, {-2, -1, 1, 2, 2, 1, -1, -2}, {1, 1, 1} };

int LOST = -1000;
int WON  =  1000;
int EMPTY = 12;

int check (int board[8][8]) {
    int result = 0;
    int oc[] = { LOST, -9, -5, -4, -3, -1, WON, 9, 5, 4, 3, 1, 0 };
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            result += oc[board[i][j]];
        }
    }

    return result;
}

int best(int board[8][8], int mode, int *x, int *y, int *dir, int *dis) {
    int px_pom, py_pom, k_pom, o_pom, px, py, dx, dy, direction, distance;
    int result, res_max, res_min, movement, killed_piece;

    result = check(board);

    if(!mode || 2 * result > WON || 2 * result < LOST) {
        return result;
    }

    if ( !(mode % 2) ) {
        for (px = 0, res_max = 100 * LOST; px < 8; px++) {
            for (py = 0; py < 8; py++) {
                if (board[px][py] >= 6 && board[px][py] < 12) {
                    for (direction = 0; direction < MAX_DIRECTION[board[px][py]]; direction++) {
                        for (distance = 1; distance < MAX_DISTANCE[board[px][py]]; distance++) {
                            dx = (distance - 1) * VEC_X[board[px][py]][direction];
                            dy = (distance - 1) * VEC_Y[board[px][py]][direction];

                            if (distance >= 2 && board[px + dx][py + dy] != EMPTY) {
                                break;
                            }

                            dx = distance * VEC_X[ board[px][py] ][direction];
                            dy = distance * VEC_Y[ board[px][py] ][direction];

                            if (px+dx >= 0 && px+dx < 8 && py+dy >= 0 && py + dy < 8) {
                                if (board[px+dx][py+dy] == EMPTY || board[px+dx][py+dy] <= 5) {
                                    if (board[px][py] != 11 || (board[px+dx][py+dy] == EMPTY && !dx)  || (board[px+dx][py+dy] == EMPTY && dx) ) {
                                        movement = board[px][py];
                                        killed_piece = board[px + dx][py + dy];
                                        board[px+dx][py+dy] = board[px][py];
                                        board[px][py] = EMPTY;

                                        if (board[px+dx][py+dy] == 11 && py+dy == 7) {
                                            board[px+dx][py+dy] = 7;
                                        }

                                        result = best(board, mode - 1, &px_pom, &py_pom, &k_pom, &o_pom);
                                        board[px][py] = movement;
                                        board[px+dx][py+dy] = killed_piece;

                                        if (result >= res_max) {
                                            res_max = result;
                                            *x = px;
                                            *y = py;
                                            *dir = direction;
                                            *dis = distance;
                                        }

                                    }
                                }

                            }

                        }
                    }
                }
            }

        }

        return res_max;
    } else {

        for (px = 0, res_min = 100 * WON; px < 8; px++) {
            for (py = 0; py < 8; py++) {
                if (board[px][py] <= 5) {
                    for (direction = 0; direction < MAX_DIRECTION[board[px][py]]; direction++) {
                        for (distance = 1; distance < MAX_DISTANCE[board[px][py]]; distance++) {
                            dx = (distance - 1) * VEC_X[board[px][py]][direction];
                            dy = (distance - 1) * VEC_Y[board[px][py]][direction];

                            if (distance >= 2 && board[px + dx][py + dy] != EMPTY) {
                                break;
                            }

                            dx = distance * VEC_X[ board[px][py] ][direction];
                            dy = distance * VEC_Y[ board[px][py] ][direction];

                            if (px+dx >= 0 && px+dx < 8 && py+dy >= 0 && py + dy < 8) {
                                if (board[px+dx][py+dy] >= 6) {
                                    if (board[px][py] != 5 || (board[px+dx][py+dy] == EMPTY && !dx)  || (board[px+dx][py+dy] == EMPTY && dx) ) {
                                        movement = board[px][py];
                                        killed_piece = board[px + dx][py + dy];
                                        board[px+dx][py+dy] = board[px][py];
                                        board[px][py] = EMPTY;

                                        if (board[px+dx][py+dy] == 5 && py+dy == 0) {
                                            board[px+dx][py+dy] = 1;
                                        }

                                        result = best(board, mode - 1, &px_pom, &py_pom, &k_pom, &o_pom);
                                        board[px][py] = movement;
                                        board[px+dx][py+dy] = killed_piece;

                                        if (result >= res_max) {
                                            res_max = result;
                                            *x = px;
                                            *y = py;
                                            *dir = direction;
                                            *dis = distance;
                                        }

                                    }
                                }

                            }

                        }
                    }
                }
            }

        }

        return res_min;

    }


}

void print(int board[8][8]) {
    //przyjmując: 0-król, 1-hetman, 2-wieża, 3-goniec, 4-skoczek,
    //5-pionek, 6-król_k, 7-hetman_k, 8-wieża_k, 9-goniec_k,
    //10-skoczek_k, 11-pionek_k, 12-pole puste

    printf("---------------------------------\n");

    for (int x = 0; x < 8; x++) {
        printf("|");
        for (int y = 0; y < 8; y++) {
            char names[] = {'K', 'H', 'W', 'G', 'S', 'P', 'k', 'h', 'w', 'g', 's', 'p', ' '};

            printf(" %c |", names[board[x][y]]);
        }
        printf("\n");
        printf("---------------------------------\n");
    }
    printf("\n");
}

int main() {

    int b[9][8] = {
            {  8, 11, 12, 12, 12, 12, 5, 2 },
            { 10, 11, 12, 12, 12, 12, 5, 4 },
            {  9, 11, 12, 12, 12, 12, 5, 3 },
            {  6, 11, 12, 12, 12, 12, 5, 0 },
            {  7, 11, 12, 12, 12, 12, 5, 1 },
            {  9, 11, 12, 12, 12, 12, 5, 3 },
            { 10, 11, 12, 12, 12, 12, 5, 4 },
            {  8, 11, 12, 12, 12, 12, 5, 2 },
    };

    int x, y, k, o, dx, dy, x2, y2;
    int result;

    while (1) {
        result = best(b, 4, &x, &y, &k, &o);
        printf("%d", result);
        /*if (result >= WON || result <= LOST) {
            return 0;
        }*/

        dx = o * VEC_X[b[x][y]][k];
        dy = o * VEC_Y[b[x][y]][k];

        b[x+dx][y+dy] = b[x][y];
        b[x][y] = EMPTY;

        if (b[x+dx][y+dy] == 11  && y+dy ==7 ) {
            b[x+dx][y+dy] = 7;
        }

        print(b);

        printf("Podaj ruch - [z rzad] [z kolumna] [do rzad] [do kolumna]:\r");
        scanf("%d %d %d %d", &x, &y, &x2, &y2);

        b[x2][y2] = b[x][y];
        b[x][y] = EMPTY;

        if (b[x2][y2] == 5 && !y2) {
            b[x2][y2] = 1;
        }

        print(b);
    }
}
