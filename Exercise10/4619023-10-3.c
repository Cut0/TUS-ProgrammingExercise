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

int N, M;                    /* 頂点数，辺数 */
int Adj[maxN + 1][maxN + 1]; /* 隣接行列を表す変数 */
int d[maxN], f[maxN], p[maxN], color[maxN];
int T; /* time を表す変数 T*/

void traverse(int u); /* 関数 traverse を宣言 */
void allVisitDfs();
void dfs(int v0);

int main() {
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
  fclose(fp);
  allVisitDfs();

  for (u = 0; u < N; u++) {
    if (d[u] == inf)
      printf("vertex %d: d[%d]=inf, p[%d]=%d\n", u, u, u, p[u]);
    else
      printf("vertex %d: d[%d]=%d, p[%d]=%d\n", u, u, d[u], u, p[u]);
  }
}

void allVisitDfs() {
  int v;
  for (v = 0; v < N; v++) {
    color[v] = 0;
    d[v] = inf;
    f[v] = inf;
    p[v] = -1;
  }
  T = 0;
  for (v = 0; v < N; v++)
    if (color[v] == 0) traverse(v);
}

void traverse(int v) {
  int u;
  color[v] = 1;
  T++;
  d[v] = T;
  for (u = 0; u < N; u++) {
    if (Adj[u][v] == 1) {
      if (color[u] == 0) {
        p[u] = v;
        traverse(u);
      }
    }
  }
  color[v] = 2;
  T++;
  f[v] = T;
}
