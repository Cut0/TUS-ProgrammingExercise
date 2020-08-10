//学籍番号: 4619023 氏名:　加藤　零
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define W 4 /* W = 二進数で表した時のけたの最大長さ，ここでは 10 に設定 */
#define m 200000 /* m = ハッシュ表のサイズ，ここでは 97 に設定 */
#define l 16
#define maxN 1000000
#define maxCell 200000
#define inf INT_MAX
int head = 0, tail = 0;

typedef struct Cell {
  int key;
  int dist;
  int parent;
  unsigned int state : 1;  // 0が空で，1が既に占有されている
} Cell;

void enqueue(int *A, int a);
int dequeue(int *A);
int hashSearch(Cell *B, int key);
void hashInsert(Cell *B, Cell Cell);
int hashVal(int a);
int powN(int a, int b);
int toDecimal(int *board);
void toBinary(int num, int *board);
int pieceCount(int boardNum);
int findNextBoards(int currentBoard, int *board);

void testBoard(int *board);
void testBoardNum(int boardNum);

int main() {
  int board[16];           // 2進数で配置を保存する配列
  int A[maxN];             //一時的にデータを保存しておくキュー
  Cell B[maxCell];         //最終的な木
  Cell leaf;               //最小コマ数の時の盤面を保存する用
  int minPieceCount = 15;  //最小のコマ数を格納する変数
  int i;

  for (i = 0; i < 4; i++)
    scanf("%d %d %d %d", &board[i * 4], &board[i * 4 + 1], &board[i * 4 + 2],
          &board[i * 4 + 3]);
  // BFSの初期設定
  int startDecimal = toDecimal(board);
  Cell startCell = {startDecimal, 0, -1, 1};
  enqueue(A, startDecimal);
  hashInsert(B, startCell);

  // Aが空なら終了
  while (head != tail) {
    int nextBoards[16];
    int currentBoardNum = dequeue(A);
    //ここで現在のセルの隣接頂点を求める
    int nextCount = findNextBoards(currentBoardNum, nextBoards);
    //隣接頂点それぞれに対しBになければAとBにinsertを繰り返す
    for (i = 0; i < nextCount; i++) {
      Cell nextCell = {nextBoards[i], 15 - pieceCount(nextBoards[i]),
                       currentBoardNum, 1};
      if (hashSearch(B, nextBoards[i]) == -1) {
        enqueue(A, nextBoards[i]);
        hashInsert(B, nextCell);
        if (minPieceCount > pieceCount(nextBoards[i])) {
          leaf = nextCell;
          minPieceCount = pieceCount(nextBoards[i]);
        }
      }
    }
  }

  Cell last = leaf;
  while (leaf.parent != -1) {
    testBoardNum(leaf.key);
    int pos = hashSearch(B, leaf.parent);
    leaf = B[pos];
  }

  /*
  for (i = 0; i < maxCell; i++) {
    if (B[i].state == 1) {
      printf("%d ", B[i].key);
      printf("%d\n", B[i].state);
    }
  }
  */
  return 0;
}

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
    head += 1;
    if (head == maxN) head = 0;
    return a;
  }
}

//ハッシュ系をまとめた------------------------------------------------------------
int hashSearch(Cell *B, int key) {
  int i = 0;
  int x = hashVal(key);
  for (i = 0; i <= m; i++) {
    int x = (hashVal(key) + i) % m;
    if (B[x].state == 1 && B[x].key == key)
      return x;
    else if (B[x].state == 0)
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
    exit(1);
  } else {
    B[x] = cell;
    B[x].state = 1;
  }
}

int hashVal(int a) {
  int i = 0, h = 0;
  for (i = W; i > 0; i--) {
    h += a / powN(10, i);
    a = a % powN(10, i);
  }
  h += a;
  return h % m;
}
//-------------------------------------------------------------------------

int toDecimal(int *board) {
  int i;
  int result = 0;
  for (i = 0; i < l; i++) {
    result += powN(board[i] * 2, i);
  }
  return result;
}

void toBinary(int num, int *board) {
  int i, base = 1;
  for (i = 0; i < l; i++) {
    board[i] = 0;
  }
  int count = 0;
  while (num > 0) {
    board[count] = (num % 2);
    num /= 2;
    base *= 10;
    count += 1;
  }
}

int powN(int a, int b) {
  int i, result = 1;
  if (a == 0 && b == 0) return 0;
  for (i = 0; i < b; i++) {
    result *= a;
  }
  return result;
}

int pieceCount(int boardNum) {
  int i, result = 0;
  int board[16];
  toBinary(boardNum, board);
  for (i = 0; i < l; i++)
    if (board[i] == 1) result += 1;
  return result;
}

// boardのsizeは16がmax
int findNextBoards(int num, int *nextBoards) {
  int count = 0;
  int i = 0;
  int currentBoard[16];
  toBinary(num, currentBoard);
  testBoard(currentBoard);
  //横に探索
  for (i = 0; i < 4; i++) {
    // 0011配置
    if (currentBoard[i * 4] == 0 && currentBoard[i * 4 + 1] == 0 &&
        currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 1) {
      nextBoards[count] = num - (powN(2, i * 4 + 1) + powN(2, i * 4 + 3));
      count += 1;
    }
    // 0111配置
    if (currentBoard[i * 4] == 0 && currentBoard[i * 4 + 1] == 1 &&
        currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 1) {
      nextBoards[count] = num - (powN(2, i * 4) + powN(2, i * 4 + 2));
      count += 1;
    }
    // 1011配置
    if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 0 &&
        currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 1) {
      nextBoards[count] = num - (powN(2, i * 4 + 1) + powN(2, i * 4 + 3));
      count += 1;
    }
    // 1100配置
    if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 1 &&
        currentBoard[i * 4 + 2] == 0 && currentBoard[i * 4 + 3] == 0) {
      nextBoards[count] = num + powN(2, i * 4);
      count += 1;
    }
    // 1101配置
    if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 1 &&
        currentBoard[i * 4 + 2] == 0 && currentBoard[i * 4 + 3] == 1) {
      nextBoards[count] = num + powN(2, i * 4);
      count += 1;
    }
    // 1110配置
    if (currentBoard[i * 4] == 1 && currentBoard[i * 4 + 1] == 1 &&
        currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 0) {
      nextBoards[count] = num + (powN(2, i * 4 + 1));
      count += 1;
    }

    // 0110配置
    if (currentBoard[i * 4] == 0 && currentBoard[i * 4 + 1] == 1 &&
        currentBoard[i * 4 + 2] == 1 && currentBoard[i * 4 + 3] == 0) {
      nextBoards[count] = num + powN(2, i * 4 + 1);
      nextBoards[count + 1] = num - (powN(2, i * 4) + powN(2, i * 4 + 2));
      count += 2;
    }
  }
  //縦に探索
  for (i = 0; i < 4; i++) {
    // 0011配置
    if (currentBoard[i] == 0 && currentBoard[i + 4] == 0 &&
        currentBoard[i + 8] == 1 && currentBoard[i + 12] == 1) {
      nextBoards[count] =
          num - (powN(2, i + 8) + powN(2, i + 12) - powN(2, i + 4));
      count += 1;
    }
    // 0111配置
    if (currentBoard[i] == 0 && currentBoard[i + 4] == 1 &&
        currentBoard[i + 8] == 1 && currentBoard[i + 12] == 1) {
      nextBoards[count] = num - (powN(2, i + 4) + powN(2, i + 8) - powN(2, i));
      count += 1;
    }
    // 1011配置
    if (currentBoard[i] == 1 && currentBoard[i + 4] == 0 &&
        currentBoard[i + 8] == 1 && currentBoard[i + 12] == 1) {
      nextBoards[count] =
          num - (powN(2, i + 8) + powN(2, i + 12) - powN(2, i + 4));
      count += 1;
    }
    // 1100配置
    if (currentBoard[i] == 1 && currentBoard[i + 4] == 1 &&
        currentBoard[i + 8] == 0 && currentBoard[i + 12] == 0) {
      nextBoards[count] = num - (powN(2, i) + powN(2, i + 4) - powN(2, i + 8));
      count += 1;
    }
    // 1101配置
    if (currentBoard[i] == 1 && currentBoard[i + 4] == 1 &&
        currentBoard[i + 8] == 0 && currentBoard[i + 12] == 1) {
      nextBoards[count] = num - (powN(2, i) + powN(2, i + 4) - powN(2, i + 8));
      count += 1;
    }
    // 1110配置
    if (currentBoard[i] == 1 && currentBoard[i + 4] == 1 &&
        currentBoard[i + 8] == 1 && currentBoard[i + 12] == 0) {
      nextBoards[count] =
          num - (powN(2, i + 4) + powN(2, i + 8) - powN(2, i + 12));
      count += 1;
    }
    // 0110配置
    if (currentBoard[i] == 0 && currentBoard[i + 4] == 1 &&
        currentBoard[i + 8] == 1 && currentBoard[i + 12] == 0) {
      nextBoards[count] =
          num - (powN(2, i + 4) + powN(2, i + 8) - powN(2, i + 12));
      nextBoards[count + 1] =
          num - (powN(2, i + 4) + powN(2, i + 8) - powN(2, i));
      count += 2;
    }
  }
  return count;
}

void testBoard(int *board) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      printf("%d ", board[i * 4 + j]);
    }
    printf("\n");
  }
  printf("\n");
}

void testBoardNum(int boardNum) {
  int i, j;
  int board[16];
  toBinary(boardNum, board);
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      printf("%d ", board[i * 4 + j]);
    }
    printf("\n");
  }
  printf("\n");
}