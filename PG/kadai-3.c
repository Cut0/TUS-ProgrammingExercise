#include <stdio.h>
#include <stdlib.h>
#define W 4    /* W = 二進数で表した時のけたの最大長さ */
#define m 2115 /* m = ハッシュ表のサイズ */
#define l 16
#define maxN 3735
#define maxCell 2115
int head = 0, tail = 0;
int powDi[17] = {
    1,   2,    4,    8,    16,   32,    64,    128,  256,
    512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};  // 2のn乗の配列関数化するよりも高速だった
int powDeca[5] = {1, 10, 100, 1000,
                  10000};  // 10のn乗の配列関数化するよりも高速だった
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
void to_binary(int num, int *board);  //数値をboard(2進数)に変換する関数
int piece_count(int boardNum);  //ボードの値から駒の数を求める関数
int rotate_board(int num);  //盤面を右に90度回転して結果の数値を返す関数
int mirror_board(int num);  //盤面を左右で反転させる関数
int find_next_boards(
    int currentBoard,
    int *nextBoardNums);  //次の盤面をboardに入れて盤面の数を返す関数
void print_board(int num);  // cellの内容を表示する関数

int main() {
    int A[maxN];          //一時的にデータを保存しておくキュー
    Cell Bn[3][maxCell];  //最終的な木
    Cell lastBoardCell[3];  //最小コマ数の時の盤面を保存する用
    int initalBoards[16] = {65534, 65503, 65533, 65527, 32767, 61439,
                            65471, 64511, 65023, 65531, 65407, 65519,
                            49151, 57343, 65279, 63487};
    int pieceCounts[3] = {0, 0, 0};  // 3津の初期配置に対するピースのカウント
    int i, j;

    for (j = 0; j < 3; j++) {
        // BFSの初期設定
        head = 0, tail = 0;
        int minpieceCount = 15;
        int startNum = initalBoards[j];
        printf("initial configuration:\n");
        print_board(startNum);
        enqueue(A, startNum);
        Cell startCell = {startNum, 0, -1, 1};
        hash_insert(Bn[j], startCell);
        // Aが空なら終了
        while (head != tail) {
            int nextBoardNums[l];
            int currentBoardNum = dequeue(A);
            //ここで現在のセルの隣接頂点を求める
            int nextCount = find_next_boards(currentBoardNum, nextBoardNums);
            //隣接頂点それぞれに対しBになければAとBにinsertを繰り返す
            for (i = 0; i < nextCount; i++) {
                int pieceCount = piece_count(nextBoardNums[i]);
                Cell nextCell = {nextBoardNums[i], 15 - pieceCount,
                                 currentBoardNum, 1};
                if (hash_search(Bn[j], nextBoardNums[i]) == -1) {
                    enqueue(A, nextBoardNums[i]);
                    hash_insert(Bn[j], nextCell);
                    if (minpieceCount > pieceCount) {
                        lastBoardCell[j] = nextCell;
                        pieceCounts[j] = pieceCount;
                        minpieceCount = pieceCount;
                    }
                }
            }
        }
        printf("last board\nvalue = %d, #pieces = %d\n", lastBoardCell[j].key,
               pieceCounts[j]);
        print_board(lastBoardCell[j].key);
        printf("\n");
    }

    //上記で求めた三つの初期配置の最終盤面を回転させたり反転させたりすることで他の初期配置の最終盤面を求めている。
    int boardNum;
    for (j = 3; j < 16; j++) {
        if (j == 3 || j == 6 || j == 9) boardNum = lastBoardCell[j / 3 - 1].key;
        printf("initial configuration:\n");
        print_board(initalBoards[j]);
        if (j >= 9 && j % 2 == 1) {
            int mirror = mirror_board(boardNum);
            printf("last board\nvalue = %d, #pieces = %d\n", mirror,
                   pieceCounts[2]);
            print_board(mirror);
        } else {
            boardNum = rotate_board(boardNum);
            printf("last board\nvalue = %d, #pieces = %d\n", boardNum,
                   pieceCounts[0]);
            print_board(boardNum);
        }
        printf("\n");
    }

    //最初に求めた三つの盤面の最終コマ数に関する手順を表示すれば良い(他の盤面は回転させたり反転させたりしただけで実質同じだから)
    int usedPiecePos[3] = {0, 0, 0};
    for (i = 0; i < 3; i++) {
        if (usedPiecePos[pieceCounts[i]] != 1) {
            printf("last piece count == %d\ntransform sequence (in reverse)\n",
                   pieceCounts[i]);
            while (1) {
                printf("value = %d, #pieces = %d\n", lastBoardCell[i].key,
                       piece_count(lastBoardCell[i].key));
                print_board(lastBoardCell[i].key);
                if (lastBoardCell[i].parent == -1) break;
                int pos = hash_search(Bn[i], lastBoardCell[i].parent);
                lastBoardCell[i] = Bn[i][pos];
            }
            usedPiecePos[pieceCounts[i]] = 1;
        }
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
    if (head == tail) {
        printf("queue underflow\n");
        exit(1);
    } else {
        int a = A[head];
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
    //ハッシュ関数まだ作ってません
    num %= m;
    return num;
}
//-------------------------------------------------------------------------

void to_binary(int num, int *board) {
    int i, base = 1;
    for (i = 0; i < l; i++) board[i] = 0;
    int count = 15;
    while (num > 0) {
        board[count] = num % 2;
        num /= 2;
        base *= 10;
        count -= 1;
    }
}

int piece_count(int boardNum) {
    int i;
    for (i = 0; boardNum != 0; i++) boardNum &= boardNum - 1;
    return i;
}

int rotate_board(int num) {
    int board[16], i;
    to_binary(num, board);
    for (i = 0; i < 16; i++)
        num += board[i] * (powDi[i / 4 + (3 - i % 4) * 4] - powDi[15 - i]);
    return num;
}

int mirror_board(int num) {
    int board[16], i;
    to_binary(num, board);
    for (i = 0; i < 16; i++)
        num += board[i] * (powDi[-12 + i + (15 - i) / 4 * 8] - powDi[15 - i]);
    return num;
}

// boardのsizeは16がmax
int find_next_boards(int num, int *nextBoardNums) {
    int count = 0;
    int i = 0;
    int currentBoard[l];
    to_binary(num, currentBoard);

    //横に探索
    for (i = 0; i < 4; i++) {
        int pos0 = powDi[15 - 4 * i];
        int pos1 = powDi[14 - 4 * i];
        int pos2 = powDi[13 - 4 * i];
        int pos3 = powDi[12 - 4 * i];
        // 0011配置
        if (currentBoard[i * 4] == 0 && currentBoard[i * 4 + 1] == 0 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 1) {
            nextBoardNums[count] = num - (pos2 + pos3 - pos1);
            count += 1;
        }
        // 0111配置
        if (currentBoard[i * 4] == 0 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 1) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos0);
            count += 1;
        }
        // 1011配置
        if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 0 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 1) {
            nextBoardNums[count] = num - (pos2 + pos3 - pos1);
            count += 1;
        }
        // 1100配置
        if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 0 && currentBoard[i * 4 + 3] == 0) {
            nextBoardNums[count] = num - (pos0 + pos1 - pos2);
            count += 1;
        }
        // 1101配置
        if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 0 && currentBoard[i * 4 + 3] == 1) {
            nextBoardNums[count] = num - (pos0 + pos1 - pos2);
            count += 1;
        }
        // 1110配置
        if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 0) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos3);
            count += 1;
        }

        // 0110配置
        if (currentBoard[i * 4] == 0 && currentBoard[i * 4 + 1] == 1 &&
            currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 0) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos0);
            nextBoardNums[count] = num - (pos1 + pos2 - pos3);
            count += 2;
        }
    }
    //縦に探索
    for (i = 0; i < 4; i++) {
        int pos0 = powDi[15 - i];
        int pos1 = powDi[11 - i];
        int pos2 = powDi[7 - i];
        int pos3 = powDi[3 - i];
        // 0011配置
        if (currentBoard[i] == 0 && currentBoard[i + 4] == 0 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 1) {
            nextBoardNums[count] = num - (pos2 + pos3 - pos1);
            count += 1;
        }
        // 0111配置
        if (currentBoard[i] == 0 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 1) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos0);
            count += 1;
        }
        // 1011配置
        if (currentBoard[i] == 1 && currentBoard[i + 4] == 0 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 1) {
            nextBoardNums[count] = num - (pos2 + pos3 - pos1);
            count += 1;
        }
        // 1100配置
        if (currentBoard[i] == 1 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 0 && currentBoard[i + 12] == 0) {
            nextBoardNums[count] = num - (pos0 + pos1 - pos2);
            count += 1;
        }
        // 1101配置
        if (currentBoard[i] == 1 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 0 && currentBoard[i + 12] == 1) {
            nextBoardNums[count] = num - (pos0 + pos1 - pos2);
            count += 1;
        }
        // 1110配置
        if (currentBoard[i] == 1 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 0) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos3);
            count += 1;
        }
        // 0110配置
        if (currentBoard[i] == 0 && currentBoard[i + 4] == 1 &&
            currentBoard[i + 8] == 1 && currentBoard[i + 12] == 0) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos3);
            nextBoardNums[count + 1] = num - (pos1 + pos2 - pos0);
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