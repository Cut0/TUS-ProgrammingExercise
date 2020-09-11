#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define m 2115 /* m = ハッシュ表のサイズ */
#define maxA 500
#define maxB 3000
int PowDi[17] = {
    1,   2,    4,    8,    16,   32,    64,    128,  256,
    512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};  // 2のn乗の配列、関数化するよりも高速だった
typedef struct Cell {
    int key;
    int dist;
    int parent;
    unsigned int state : 1;  // 0が空で，1が既に占有されている
} Cell;

//キューに挿入する関数
void enqueue(int *A, int a);
//キューの先頭を取り出す関数
int dequeue(int *A);

// keyに一致する要素の場所を返す関数
int hash_search(Cell *B, int key);
// cellを表に挿入する関数
void hash_insert(Cell *B, Cell Cell);
//ハッシュ値を返す関数
int hash_value(int num);
//次の盤面をnextBoardNumsに入れて盤面の数を返す関数
int find_next_boards(int currentBoard, int *nextBoardNums);
// initialBoardNumを基にBFSする最終盤面のセルが返ってくる。
Cell bfs(Cell *B, int initalBoardNum);

//数値をboard(2進数)に変換する関数
void to_binary(int num, int *board);
//ボードの値から駒の数を求める関数
int piece_count(int boardNum);
//盤面を右に90度回転した結果の数値を返す関数
int rotate_board(int num);
//盤面を左右で反転させた結果の数値を返す関数
int mirror_board(int num);
// numに対応する盤面を表示する関数
void print_board(int num);

// lastBoard系統はポインタ渡しで他の関数に渡しちゃダメ、ここの関数内だけで書き換えをする
int main() {
    int initalBoardNums[16] = {65534, 65527, 32767, 61439, 65503, 65471,
                               64511, 65023, 65533, 65407, 49151, 65279,
                               65531, 65519, 57343, 63487};
    //最終的な木
    Cell Bn[3][maxB];
    // 3種類の初期配置に対する最小コマ数の盤面(他の13種類はこれを回転したり反転したりすれば求められる)
    Cell lastBoardCell[3];
    int lastBoardNums[16];
    int i;

    clock_t startT = clock();
    // BFSを使って木を構築した後、ベースとなる3種類の初期配置に対応する最終盤面を求めている
    for (i = 0; i < 3; i++) {
        lastBoardCell[i] = bfs(Bn[i], initalBoardNums[i * 4]);
        lastBoardNums[i * 4] = lastBoardCell[i].key;
    }

    //回転させて最終盤面を求める
    for (i = 1; i < 12; i++) {
        if (i % 4 == 0) continue;
        lastBoardNums[i] = rotate_board(lastBoardNums[i - 1]);
    }
    //反転させて最終盤面を求める
    for (i = 12; i < 16; i++)
        lastBoardNums[i] = mirror_board(lastBoardNums[i - 4]);

    clock_t endT = clock();

    //それぞれの初期値に対する最終盤面の表示
    for (i = 0; i < 16; i++) {
        printf("initial configuration:\n");
        print_board(initalBoardNums[i]);
        printf("last board\nvalue = %d, #pieces = %d\n", lastBoardNums[i],
               15 - lastBoardCell[i < 12 ? i / 4 : 2].dist);
        print_board(lastBoardNums[i]);
        printf("\n");
    }

    // 3種類の初期値に対する最終盤面の手順を表示
    for (i = 0; i < 3; i++) {
        printf("last piece count == %d\ntransform sequence (in reverse)\n",
               15 - lastBoardCell[i].dist);
        while (1) {
            printf("value = %d, #pieces = %d\n", lastBoardCell[i].key,
                   15 - lastBoardCell[i].dist);
            print_board(lastBoardCell[i].key);
            if (lastBoardCell[i].parent == -1) break;
            int pos = hash_search(Bn[i], lastBoardCell[i].parent);
            lastBoardCell[i] = Bn[i][pos];
        }
    }
    printf(" time =  %lf\n", (double)(endT - startT) / CLOCKS_PER_SEC);
    return 0;
}

//キュー系をまとめた------------------------------------------------------------
int Head = 0, Tail = 0;
void enqueue(int *A, int a) {
    A[Tail] = a;
    Tail += 1;
    if (Tail == maxA) Tail = 0;
    if (Tail == Head) {
        printf("queue overflow\n");
        exit(1);
    }
}

int dequeue(int *A) {
    if (Head == Tail) {
        printf("queue underflow\n");
        exit(1);
    } else {
        int a = A[Head];
        A[Head] = -1;
        Head += 1;
        if (Head == maxA) Head = 0;
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
        if (B[(h + i) % m].state != 1)
            x = (h + i) % m;
        else
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
    num %= m;
    return num;
}

// boardのsizeは16がmax
int find_next_boards(int num, int *nextBoardNums) {
    int count = 0;
    int i = 0;
    int currentBoard[16];
    to_binary(num, currentBoard);

    //横に探索
    for (i = 0; i < 4; i++) {
        int pos0 = PowDi[15 - 4 * i];
        int pos1 = PowDi[14 - 4 * i];
        int pos2 = PowDi[13 - 4 * i];
        int pos3 = PowDi[12 - 4 * i];
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
        int pos0 = PowDi[15 - i];
        int pos1 = PowDi[11 - i];
        int pos2 = PowDi[7 - i];
        int pos3 = PowDi[3 - i];
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

Cell bfs(Cell *B, int initalBoardNum) {
    //一時的にデータを保存しておくキュー
    int A[maxA];
    Cell lastBoardCell;
    int i;
    // BFSの初期設定
    Head = 0, Tail = 0;
    int minpieceCount = 15;
    enqueue(A, initalBoardNum);
    Cell startCell = {initalBoardNum, 0, -1, 1};
    hash_insert(B, startCell);
    // Aが空なら終了
    while (Head != Tail) {
        //次の盤面が全て１０進数で格納される配列
        int nextBoardNums[16];
        int currentBoardNum = dequeue(A);
        //ここで現在のセルの隣接頂点を求める
        int nextCount = find_next_boards(currentBoardNum, nextBoardNums);
        //隣接頂点それぞれに対しBになければAとBにinsertを繰り返す
        int count = 0;
        for (i = 0; i < nextCount; i++) {
            int nextPieceCount = piece_count(nextBoardNums[i]);
            Cell nextCell = {nextBoardNums[i], 15 - nextPieceCount,
                             currentBoardNum, 1};
            if (hash_search(B, nextBoardNums[i]) == -1) {
                enqueue(A, nextBoardNums[i]);
                hash_insert(B, nextCell);
                if (minpieceCount > nextPieceCount) {
                    lastBoardCell = nextCell;
                    minpieceCount = nextPieceCount;
                }
            }
        }
    }
    return lastBoardCell;
}
//-------------------------------------------------------------------------

void to_binary(int num, int *board) {
    int i;
    for (i = 0; i < 16; i++) board[i] = 0;
    int count = 15;
    while (num > 0) {
        board[count] = num % 2;
        num /= 2;
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
        num += board[i] * (PowDi[i / 4 + (3 - i % 4) * 4] - PowDi[15 - i]);
    return num;
}

int mirror_board(int num) {
    int board[16], i;
    to_binary(num, board);
    for (i = 0; i < 16; i++)
        num += board[i] * (PowDi[-12 + i + (15 - i) / 4 * 8] - PowDi[15 - i]);
    return num;
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