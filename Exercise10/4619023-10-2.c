//学籍番号: 4619023 氏名:　加藤　零
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define maxN 50
#define inf INT_MAX

int head = 0, tail = 0;

void enqueue(int *Q, int a) {
  Q[tail] = a;
  tail += 1;
  if (tail == maxN) tail = 0;
  if (tail == head) {
    printf("queue overflowz\n");
    exit(1);
  }
}
int dequeue(int *Q) {
  int a;
  if (head == tail) {
    printf("queue underflow\n");
    exit(1);
  } else {
    a = Q[head];
    head += 1;
    if (head == maxN) head = 0;
    return a;
  }
}

int main() {
  int N, M;            /* 頂点数，辺数 */
  int Adj[maxN][maxN]; /* 隣接行列を表す変数 */
  int d[maxN], p[maxN], color[maxN];
  int v0;      /* 始点の変数 */
  int Q[maxN]; /* キューを表す変数 */

  char fname[128]; /* 読み込むファイルの名前 */
  FILE *fp;        /* 入力ファイル */
  int u, v, i;

  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
  fp = fopen(fname, "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d %d", &N, &M);
  if (N > maxN) {
    printf("N is too large, setting N = %d\n", maxN);
    N = maxN; /* N が 50 を超えるときは警告をした上で */
  }
  for (u = 0; u < N; u++) { /* 隣接行列の初期化 */
    for (v = 0; v < N; v++) {
      Adj[u][v] = 0;
    }
  }
  for (i = 0; i < M; i++) {
    fscanf(fp, " %d %d", &u, &v); /* 順番に辺の両端 u, v を読み込む */
    Adj[u][v] += 1;               /* 隣接行列に対する処理 */
    Adj[v][u] += 1;
  }
  fscanf(fp, " %d", &v0);
  fclose(fp);

  for (u = 0; u < N; u++) {
    if (u == v0) continue;
    color[u] = 0;
    d[u] = inf;
    p[u] = -1;
  }
  color[v0] = 1;
  d[v0] = 0;
  p[v0] = -1;
  enqueue(Q, v0);

  while (head != tail) {
    v = dequeue(Q);
    for (u = 0; u < N; u++) {
      if (Adj[u][v] == 1 && color[u] == 0) {
        enqueue(Q, u);
        color[u] = 1;
        d[u] = d[v] + 1;
        p[u] = v;
      }
    }
    color[v] = 2;
  }
  for (u = 0; u < N; u++) {
    if (d[u] == inf)
      printf("vertex %d: d[%d]=inf, p[%d]=%d\n", u, u, u, p[u]);
    else
      printf("vertex %d: d[%d]=%d, p[%d]=%d\n", u, u, d[u], u, p[u]);
  }
}