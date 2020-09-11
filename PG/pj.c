#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define m 16384 /* m = ハッシュ表のサイズ */
#define maxA 500
#define maxB 65536
int PowerDouble[17] = {
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
int hash_value(int num, int i);
//次の盤面をnextBoardNumsに入れて盤面の数を返す関数
int find_next_boards(int currentBoard, int *nextBoardNums);
// initialBoardNumを基にBFSする最終盤面のセルが返ってくる。
Cell bfs(Cell *B, int initialBoardNum);

//ボードの値から駒の数を求める関数
int piece_count(int boardNum);
//盤面を右に90度回転した結果の数値を返す関数
int rotate_board(int num);
//盤面を左右で反転させた結果の数値を返す関数
int mirror_board(int num);
// numに対応する盤面を表示する関数
void print_board(int num, int value, int pieceCount);

// lastBoard系統はポインタ渡しで他の関数に渡しちゃダメ、ここの関数内だけで書き換えをする
int main() {
    int initialBoardNums[16] = {
        0b1111111111111110, 0b1111111111110111, 0b0111111111111111,
        0b1110111111111111, 0b1111111111011111, 0b1111111110111111,
        0b1111101111111111, 0b1111110111111111, 0b1111111111111101,
        0b1111111101111111, 0b1011111111111111, 0b1111111011111111,
        0b1111111111111011, 0b1111111111101111, 0b1101111111111111,
        0b1111011111111111};
    //最終的な木
    Cell Bn[3][maxB];
    // 3種類の初期配置に対する最小コマ数の盤面(他の13種類はこれを回転したり反転したりすれば求められる)
    Cell lastBoardCells[3];
    int lastBoardNums[16];
    int i, j;

    clock_t startT = clock();
    // BFSを使って木を構築した後、ベースとなる3種類の初期配置に対応する最終盤面を求めている
    for (i = 0; i < 3; ++i) {
        lastBoardCells[i] = bfs(Bn[i], initialBoardNums[i * 4]);
        lastBoardNums[i * 4] = lastBoardCells[i].key;
    }
    clock_t endT = clock();
    //以下表示なので時間計測に含めない
    //回転させて最終盤面を求める
    for (i = 1; i < 12; ++i) {
        if (i % 4 == 0) continue;
        lastBoardNums[i] = rotate_board(lastBoardNums[i - 1]);
    }
    //反転させて最終盤面を求める
    for (i = 12; i < 16; ++i)
        lastBoardNums[i] = mirror_board(lastBoardNums[i - 4]);

    //それぞれの初期値に対する最終盤面の表示
    for (i = 0; i < 16; ++i) {
        printf("initial configuration\n");
        print_board(initialBoardNums[i], initialBoardNums[i], 15);
        printf("last board\n");
        print_board(lastBoardNums[i], lastBoardNums[i],
                    15 - lastBoardCells[i < 12 ? i / 4 : 2].dist);
        printf("\n");
    }

    // 3種類の初期値に対する最終盤面の手順を表示
    for (i = 0; i < 3; ++i) {
        printf("last piece count == %d\ntransform sequence (in reverse)\n",
               15 - lastBoardCells[i].dist);
        while (1) {
            print_board(lastBoardCells[i].key, lastBoardCells[i].key,
                        15 - lastBoardCells[i].dist);
            if (lastBoardCells[i].parent == -1) break;
            int pos = hash_search(Bn[i], lastBoardCells[i].parent);
            lastBoardCells[i] = Bn[i][pos];
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
    for (i = 0; i <= m; ++i) {
        int x = (hash_value(key, i) % m);
        if (B[x].state == 1 && B[x].key == key)
            return x;
        else if (B[x].state == 0)
            return -1;
    }
    return -1;
}

void hash_insert(Cell *B, Cell cell) {
    int h = hash_value(cell.key, 0);
    int i = 0;
    while (1) {
        if (i > m) {
            printf("error: out of space\n");
            exit(1);
        }
        if (B[h].state != 1) {
            break;
        } else {
            i += 1;
            h = hash_value(cell.key, i);
        }
    }
    B[h] = cell;
    B[h].state = 1;
}

int hash_value(int num, int i) { return ((num % m) + i / 2 + i * i / 2) % m; }

// boardのsizeは16がmax
int find_next_boards(int num, int *nextBoardNums) {
    int count = 0, i;
    int slideRowNum = 0b0000000000001111;
    int slideColumnNum = 0b1000100010001000;
    for (i = 0; i < 4; ++i) {
        //横に探索
        int pos0 = PowerDouble[4 * i + 3];
        int pos1 = PowerDouble[4 * i + 2];
        int pos2 = PowerDouble[4 * i + 1];
        int pos3 = PowerDouble[4 * i];
        int detectRowNum = (num & slideRowNum) >> 4 * i;
        // n011配置
        if (detectRowNum == 0b11 || detectRowNum == 0b1011) {
            nextBoardNums[count] = num - (pos2 + pos3 - pos1);
            count += 1;
        }
        // n110配置
        if (detectRowNum == 0b110 || detectRowNum == 0b1110) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos3);
            count += 1;
        }
        // 011n配置
        if (detectRowNum == 0b110 || detectRowNum == 0b111) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos0);
            count += 1;
        }
        // 110n配置
        if (detectRowNum == 0b1100 || detectRowNum == 0b1101) {
            nextBoardNums[count] = num - (pos0 + pos1 - pos2);
            count += 1;
        }
        slideRowNum <<= 4;
        //縦に探索
        pos0 = PowerDouble[15 - i];
        pos1 = PowerDouble[11 - i];
        pos2 = PowerDouble[7 - i];
        pos3 = PowerDouble[3 - i];
        int detectColumnNum = (num & slideColumnNum) << i;
        // n011配置
        if (detectColumnNum == 0b1000000010001000 ||
            detectColumnNum == 0b10001000) {
            nextBoardNums[count] = num - (pos2 + pos3 - pos1);
            count += 1;
        }
        // n110配置
        if (detectColumnNum == 0b1000100010000000 ||
            detectColumnNum == 0b100010000000) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos3);
            count += 1;
        }
        // 011n配置
        if (detectColumnNum == 0b100010001000 ||
            detectColumnNum == 0b100010000000) {
            nextBoardNums[count] = num - (pos1 + pos2 - pos0);
            count += 1;
        }
        // 110n配置
        if (detectColumnNum == 0b1000100000001000 ||
            detectColumnNum == 0b1000100000000000) {
            nextBoardNums[count] = num - (pos0 + pos1 - pos2);
            count += 1;
        }
        slideColumnNum >>= 1;
    }
    return count;
}

Cell bfs(Cell *B, int initialBoardNum) {
    //一時的にデータを保存しておくキュー
    int A[maxA], i;
    Cell lastBoardCell;
    // BFSの初期設定
    Head = 0, Tail = 0;
    int minPieceCount = 15;
    enqueue(A, initialBoardNum);
    Cell startCell = {initialBoardNum, 0, -1, 1};
    hash_insert(B, startCell);
    // Aが空なら終了
    while (Head != Tail) {
        //次の盤面が全て１０進数で格納される配列
        int nextBoardNums[16];
        int currentBoardNum = dequeue(A);
        //ここで現在のセルの隣接頂点を求める
        int nextBoardCount = find_next_boards(currentBoardNum, nextBoardNums);
        //隣接頂点それぞれに対しBになければAとBにinsertを繰り返す
        int count = 0;
        for (i = 0; i < nextBoardCount; ++i) {
            if (hash_search(B, nextBoardNums[i]) == -1) {
                int nextPieceCount = piece_count(nextBoardNums[i]);
                Cell nextCell = {nextBoardNums[i], 15 - nextPieceCount,
                                 currentBoardNum, 1};
                enqueue(A, nextBoardNums[i]);
                hash_insert(B, nextCell);
                if (minPieceCount > nextPieceCount) {
                    lastBoardCell = nextCell;
                    minPieceCount = nextPieceCount;
                }
            }
        }
    }
    return lastBoardCell;
}
//-------------------------------------------------------------------------

int piece_count(int boardNum) {
    int i;
    for (i = 0; boardNum != 0; ++i) boardNum &= boardNum - 1;
    return i;
}

int rotate_board(int num) {
    int slideNum = 0b1000000000000000, res = 0, i;
    for (i = 0; i < 16; ++i) {
        if (num & slideNum)
            res += PowerDouble[i / 4 + (3 - i % 4) * 4] - PowerDouble[15 - i];
        slideNum >>= 1;
    }
    return res + num;
}

int mirror_board(int num) {
    int slideNum = 0b1000000000000000, res = 0, i;
    for (i = 0; i < 16; ++i) {
        if (num & slideNum)
            res +=
                PowerDouble[-12 + i + (15 - i) / 4 * 8] - PowerDouble[15 - i];
        slideNum >>= 1;
    }
    return res + num;
}

void print_board(int num, int value, int pieceCount) {
    int i, j, slideNum = 0b1000000000000000;
    printf("value = %d, #pieces = %d\n", value, pieceCount);
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            int res = num & slideNum ? 1 : 0;
            printf("%d ", res);
            slideNum >>= 1;
        }
        printf("\n");
    }
    printf("\n");
}