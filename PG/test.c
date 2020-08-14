//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define W 5
int powDi[17] = {
    1,   2,    4,    8,    16,   32,    64,    128,  256,
    512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};  // 2のn乗の配列関数化するよりも高速だった
int powDeca[5] = {1, 10, 100, 1000};
void to_binary(int num, int *board) {
    int i, base = 1;
    for (i = 0; i < 16; i++) board[i] = 0;
    int count = 0;
    while (num > 0) {
        board[count] = (num % 2);
        num /= 2;
        base *= 10;
        count += 1;
    }
}
void print_board(int num) {
    int i, j;
    int board[16];
    to_binary(num, board);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%d ", board[i * 4 + j]);
        }
        printf("\n");
    }
    printf("\n");
}
int rotate_board(int num) {
    int board[16], i;
    to_binary(num, board);
    for (i = 0; i < 16; i++) {
        if (i >= 12)
            num += board[i] * (powDi[(i - 12) * 4] - powDi[i]);
        else if (i >= 8)
            num += board[i] * (powDi[(i - 8) * 4 + 1] - powDi[i]);
        else if (i >= 4)
            num += board[i] * (powDi[(i - 4) * 4 + 2] - powDi[i]);
        else
            num += board[i] * (powDi[i * 4 + 3] - powDi[i]);
    }
    return num;
}

int mirror_board(int num) {
    int board[16], i;
    to_binary(num, board);
    for (i = 0; i < 16; i++)
        num += board[i] * (powDi[3 - i + i / 4 * 8] - powDi[i]);
    return num;
}

int main() {
    int i;
    int initalBoards[16] = {65534, 65533, 65503, 65527, 32767, 61439,
                            65471, 64511, 65023, 65531, 65407, 65519,
                            49151, 57343, 65279, 63487};
    for (i = 0; i < 16; i++) {
        printf("%d\n", initalBoards[i]);
        print_board(initalBoards[i]);
    }
    return 0;
}
