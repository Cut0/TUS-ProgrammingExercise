#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define W 4    /* W = 二進数で表した時のけたの最大長さ */
#define m 2115 /* m = ハッシュ表のサイズ */
#define l 16
#define maxN 3735
#define maxCell 2117
int head = 0, tail = 0;

typedef struct Cell {
    int key;
    int dist;
    int parent;
    unsigned int state : 1;  // 0が空で，1が既に占有されている
} Cell;

void enqueue(int *A, int a);  //キューに挿入する関数
int dequeue(int *A);          //キューの先頭を取り出す関数
int hash_search(Cell *B,
                int key);  // keyに一致する要素の場所を返す関数
void hash_insert(Cell *B, Cell Cell);  // cellを表に挿入する関数
int hash_value(int num);               //ハッシュ値を返す関数
int pow_n(int a, int b);               // aのb乗を返す関数
void to_binary(int num, int *board);   //数値をboardに変換する関数
int piece_count(int boardNum);  //ボードの値から駒の数を求める関数
int find_next_boards(int currentBoard,
                     int *board);  //次の盤面をboardに入れて盤面の数を返す関数
void print_board(int num);  // cellの内容を表示する関数

int main() {
    int A[maxN];          //一時的にデータを保存しておくキュー
    Cell Bn[3][maxCell];  //最終的な木
    Cell leaves[3];       //最小コマ数の時の盤面を保存する用
    int minpiece_count = 15;  //最小のコマ数を格納する変数
    int initalBoards[16] = {65534, 65533, 65531, 65527, 65519, 65503,
                            65471, 65407, 65279, 65023, 64511, 63487,
                            61439, 57343, 49151, 32767};
    int tests[3] = {65534, 65533, 65503};
    int i, j;

    // Aが空なら終了
    for (j = 0; j < 3; j++) {
        // BFSの初期設定
        head = 0, tail = 0;
        minpiece_count = 15;
        int startDecimal = tests[j];
        printf("initial configuration:\n");
        print_board(startDecimal);
        Cell startCell1 = {startDecimal, 0, -1, 1};
        enqueue(A, startDecimal);
        hash_insert(Bn[j], startCell1);
        while (head != tail) {
            int nextBoards[l];
            int currentBoardNum = dequeue(A);
            //ここで現在のセルの隣接頂点を求める
            int nextCount = find_next_boards(currentBoardNum, nextBoards);
            //隣接頂点それぞれに対しBになければAとBにinsertを繰り返す
            for (i = 0; i < nextCount; i++) {
                int pieceCount = piece_count(nextBoards[i]);
                Cell nextCell = {nextBoards[i], 15 - pieceCount,
                                 currentBoardNum, 1};
                if (hash_search(Bn[j], nextBoards[i]) == -1) {
                    enqueue(A, nextBoards[i]);
                    hash_insert(Bn[j], nextCell);
                    if (minpiece_count > pieceCount) {
                        leaves[j] = nextCell;
                        minpiece_count = pieceCount;
                    }
                }
            }
        }
        printf("last board\nvalue = %d, #pieces = %d\n", leaves[j].key,
               piece_count(leaves[j].key));
        print_board(leaves[j].key);
        printf("\n");
    }

    return 0;
}

//キュー系をまとめた------------------------------------------------------------
void enqueue(int *A, int a) {
    A[tail] = a;
    tail += 1;
    if (tail == maxN) tail = 0;
    if (tail == head) {
        printf("queue overflow\n");
        exit(1);
    }
}

int dequeue(int *A) {
    int a;
    if (head == tail) {
        printf("queue underflow\n");
        exit(1);
    } else {
        a = A[head];
        A[head] = -1;
        head += 1;
        if (head == maxN) head = 0;
        return a;
    }
}
//--------------------------------------------------------------------------

//ハッシュ系をまとめた------------------------------------------------------------
int hash_search(Cell *B, int key) {
    int i = 0;
    int x = hash_value(key);
    for (i = 0; i <= m; i++) {
        int x = (hash_value(key) + i) % m;
        if (B[x].state == 1 && B[x].key == key)
            return x;
        else if (B[x].state == 0)
            return -1;
    }
    return -1;
}

void hash_insert(Cell *B, Cell cell) {
    int h = hash_value(cell.key);
    int x = -1;
    int i = 0;
    while (x == -1 && i < m) {
        if (B[(h + i) % m].state != 1) {
            x = (h + i) % m;
        } else
            i += 1;
    }
    if (x == -1) {
        printf("error: out of space\n");
        exit(1);
    } else {
        B[x] = cell;
        B[x].state = 1;
    }
}

int hash_value(int num) {
    int i = 0, h = 0;
    for (i = W; i > 0; i--) {
        int powI = pow_n(10, i);
        h += num / powI;
        num = num % powI;
    }
    h += num;
    return h % m;
}
//-------------------------------------------------------------------------

void to_binary(int num, int *board) {
    int i, base = 1;
    for (i = 0; i < l; i++) board[i] = 0;
    int count = 0;
    while (num > 0) {
        board[count] = (num % 2);
        num /= 2;
        base *= 10;
        count += 1;
    }
}

int pow_n(int a, int b) {
    int i, result = 1;
    if (a == 0 && b == 0) return 0;
    for (i = 0; i < b; i++) {
        result *= a;
    }
    return result;
}

int piece_count(int boardNum) {
    int i, result = 0;
    int board[l];
    to_binary(boardNum, board);
    for (i = 0; i < l; i++)
        if (board[i] == 1) result += 1;
    return result;
}

// boardのsizeは16がmax
int find_next_boards(int num, int *nextBoards) {
    int count = 0;
    int i = 0;
    int currentBoard[l];
    to_binary(num, currentBoard);

    //横に探索
    for (i = 0; i < 4; i++) {
        int pos0 = pow_n(2, i * 4);
        int pos1 = pow_n(2, i * 4 + 1);
        int pos2 = pow_n(2, i * 4 + 2);
        int pos3 = pow_n(2, i * 4 + 3);
        // 0011配置
        if (currentBoard[i * 4] == 0 && currentBoard[i * 4 + 1] == 0 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 1) {
            nextBoards[count] = num - (pos1 + pos3);
            count += 1;
        }
        // 0111配置
        if (currentBoard[i * 4] == 0 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 1) {
            nextBoards[count] = num - (pos0 + pos2);
            count += 1;
        }
        // 1011配置
        if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 0 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 1) {
            nextBoards[count] = num - (pos1 + pos3);
            count += 1;
        }
        // 1100配置
        if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 0 && currentBoard[i * 4 + 3] == 0) {
            nextBoards[count] = num + pos0;
            count += 1;
        }
        // 1101配置
        if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 0 && currentBoard[i * 4 + 3] == 1) {
            nextBoards[count] = num + pos0;
            count += 1;
        }
        // 1110配置
        if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 0) {
            nextBoards[count] = num + pos1;
            count += 1;
        }

        // 0110配置
        if (currentBoard[i * 4] == 0 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 0) {
            nextBoards[count] = num + pos1;
            nextBoards[count + 1] = num - (pos0 + pos2);
            count += 2;
        }
    }
    //縦に探索
    for (i = 0; i < 4; i++) {
        // 0011配置
        int pos0 = pow_n(2, i);
        int pos1 = pow_n(2, i + 4);
        int pos2 = pow_n(2, i + 8);
        int pos3 = pow_n(2, i + 12);
        if (currentBoard[i] == 0 && currentBoard[i + 4] == 0 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 1) {
            nextBoards[count] = num - (pos2 + pos3 - pos1);
            count += 1;
        }
        // 0111配置
        if (currentBoard[i] == 0 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 1) {
            nextBoards[count] = num - (pos1 + pos2 - pos0);
            count += 1;
        }
        // 1011配置
        if (currentBoard[i] == 1 && currentBoard[i + 4] == 0 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 1) {
            nextBoards[count] = num - (pos2 + pos3 - pos1);
            count += 1;
        }
        // 1100配置
        if (currentBoard[i] == 1 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 0 && currentBoard[i + 12] == 0) {
            nextBoards[count] = num - (pos0 + pos1 - pos2);
            count += 1;
        }
        // 1101配置
        if (currentBoard[i] == 1 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 0 && currentBoard[i + 12] == 1) {
            nextBoards[count] = num - (pos0 + pos1 - pos2);
            count += 1;
        }
        // 1110配置
        if (currentBoard[i] == 1 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 0) {
            nextBoards[count] = num - (pos1 + pos2 - pos3);
            count += 1;
        }
        // 0110配置
        if (currentBoard[i] == 0 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 0) {
            nextBoards[count] = num - (pos1 + pos2 - pos3);
            nextBoards[count + 1] = num - (pos1 + pos2 - pos0);
            count += 2;
        }
    }
    return count;
}

void print_board(int num) {
    int i, j;
    int board[l];
    to_binary(num, board);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("%d ", board[i * 4 + j]);
        }
        printf("\n");
    }
    printf("\n");
}