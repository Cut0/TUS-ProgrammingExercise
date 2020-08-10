//学籍番号: 4619023 氏名:　加藤　零
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define W 4 /* W = 二進数で表した時のけたの最大長さ，ここでは 10 に設定 */
#define m 97 /* m = ハッシュ表のサイズ，ここでは 97 に設定 */
#define l 16
#define maxN 50
#define inf INT_MAX
int head = 0, tail = 0;

typedef struct Cell {
  int key;
  int dist;
  int parent;
  unsigned int state : 1;  // 0が空で，1が既に占有されている
} Cell;

void enqueue(Cell *A, Cell cell);
Cell dequeue(Cell *A);
int hashSearch(Cell *B, Cell cell);
void hashInsert(Cell *B, Cell Cell);
int hashVal(int a);
int toDecimal(int *board);
int findNextBoard(int *board);

int main() {
  int board[16];        // 2進数で配置を保存する配列
  struct Cell A[maxN];  //一時的にデータを保存しておくキュー
  struct Cell B[inf];   //最終的な木
  int i;
  for (i = 0; i < 4; i++)
    scanf("%d %d %d %d", &board[i * 4], &board[i * 4 + 1], &board[i * 4 + 2],
          &board[i * 4 + 3]);
  // BFSの初期設定
  int startDecimal = toDecimal(board);
  Cell startCell = {startDecimal, -1, 0, 1};
  enqueue(A, startCell);
  hashInsert(B, startCell);

  while (head != tail) {
    Cell currentCell = dequeue(A);
    //ここで現在のセルの隣接頂点を求める
    int nextBoard[16];
    for (i = 0; i < 16; i++) nextBoard[i] = -1;
    int nextCount = findNextBoard(nextBoard);
    //隣接頂点それぞれに対しBになければAとBにinsertを繰り返す
    for (i = 0; i < nextCount; i++) {
      Cell nextCell = {nextBoard[i], currentCell.dist + 1, currentCell.key, 1};
      if (hashSearch(B, nextCell) == -1) {
        enqueue(A, nextCell);
        hashInsert(B, nextCell);
      }
    }
    // Aが空なら終了
  }
  return 0;
}

void enqueue(Cell *A, Cell cell) {
  A[tail] = cell;
  tail += 1;
  if (tail == maxN) tail = 0;
  if (tail == head) {
    printf("queue overflow\n");
    exit(1);
  }
}

Cell dequeue(Cell *A) {
  Cell a;
  if (head == tail) {
    printf("queue underflow\n");
    exit(1);
  } else {
    a = A[head];
    head += 1;
    if (head == maxN) head = 0;
    return a;
  }
}

//ハッシュ系をまとめた------------------------------------------------------------
int hashSearch(Cell *B, Cell cell) {
  int i = 0;
  int x = hashVal(cell.key);
  for (i = 0; i <= m; i++) {
    int x = (hashVal(cell.key) + i) % m;
    unsigned int state = B[x].state;
    if (state == 1 && B[x].key == cell.key && B[x].dist == cell.dist &&
        B[x].parent == cell.parent)
      return x;
    else
      return -1;
  }
  return -1;
}

void hashInsert(Cell *B, Cell cell) {
  int h = hashVal(cell.key);
  int x = -1;
  int i = 0;
  while (x == -1 && i < m) {
    if (B[(h + i) % m].state != 1) {
      x = (h + i) % m;
    } else {
      i += 1;
    }
  }
  if (x == -1) {
    printf("error: out of space\n");
  } else {
    B[x] = cell;
    B[x].state = 1;
  }
}

int hashVal(int a) {
  int i = 0, h = 0;
  for (i = W; i > 0; i--) {
    h += a / pow(10, i);
    a = a % (int)pow(10, i);
  }
  h += a;
  return h % m;
}
//-------------------------------------------------------------------------

int toDecimal(int *board) {
  int i;
  int result = 0;
  for (i = 0; i < l; i++) result += pow(board[i] * 2, i);
  return i;
}

// boardのsizeは16がmax
int findNextBoard(int *board) {
  int count = 0;
  int i = 0;
  int num = toDecimal(board);
  //横に探索
  for (i = 0; i < 4; i++) {
    // 1110配置
    if (board[i * 4] == 1 && board[i * 4 + 1] == 1 && board[i * 4 + 2] == 1 &&
        board[i * 4 + 3] == 0) {
      board[count] = num - (pow(2, i * 4 + 2) + pow(2, i * 4));
      count += 1;
    }
    // 1100配置
    if (board[i * 4] == 1 && board[i * 4 + 1] == 1 && board[i * 4 + 2] == 0 &&
        board[i * 4 + 3] == 0) {
      board[count] = num - (pow(2, i * 4 + 3) + pow(2, i * 4 + 1));
      count += 1;
    }
    // 1101配置
    if (board[i * 4] == 1 && board[i * 4 + 1] == 1 && board[i * 4 + 2] == 0 &&
        board[i * 4 + 3] == 1) {
      board[count] = num - (pow(2, i * 4 + 3) + pow(2, i * 4 + 1));
      count += 1;
    }
    // 0110配置
    if (board[i * 4] == 0 && board[i * 4 + 1] == 1 && board[i * 4 + 2] == 1 &&
        board[i * 4 + 3] == 0) {
      board[count] = num - (pow(2, i * 4 + 2) + pow(2, i * 4));
      board[count + 1] = num + pow(2, i * 4 + 1);
      count += 2;
    }
  }
  //縦に探索
  for (i = 0; i < 4; i++) {
    if (board[i] == 1 && board[i + 4] == 1 && board[i + 8] == 1 &&
        board[i + 12] == 0) {
      board[count] = num - (pow(2, i + 12) + pow(2, i + 4));
      count += 1;
    }
    if (board[i] == 1 && board[i + 4] == 1 && board[i + 8] == 0 &&
        board[i + 12] == 0) {
      board[count] = num - (pow(2, i + 8) + pow(2, i));
      count += 1;
    }
    if (board[i] == 1 && board[i + 4] == 1 && board[i + 8] == 0 &&
        board[i + 12] == 1) {
      board[count] = num - (pow(2, i + 8) + pow(2, i));
      count += 1;
    }
    if (board[i] == 0 && board[i + 4] == 1 && board[i + 8] == 1 &&
        board[i + 12] == 0) {
      board[count] = num - (pow(2, i + 12) + pow(2, i + 4));
      board[count + 1] = num - (pow(2, i + 8) + pow(2, i + 4));
      count += 2;
    }
  }
  return count;
}