//学籍番号: 4619023 氏名:　加藤　零
#define maxN 50
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define maxN 50
#define inf INT_MAX

int main() {
  int N, M;             /* 頂点数，辺数 */
  int Lmat[maxN][maxN]; /* 重み行列を表す変数 */
  int d[maxN], p[maxN]; /* d[u], p[u] を格納する変数 */
  int A[maxN];
  int v0;          /* 始点を表す変数 */
  char fname[128]; /* 読み込むファイルの名前 */
  FILE *fp;        /* 入力ファイル */
  int u, v, i, j;

  for (i = 0; i < maxN; i++) {
    A[i] = 0;
    for (j = 0; j < maxN; j++) Lmat[i][j] = 0;
  };

  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
  fp = fopen("graph1.txt", "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d %d", &N, &M);
  if (N > maxN) {
    printf("N is too large, setting N = %d\n", maxN);
    N = maxN; /* N が 50 を超えるときは警告をした上で */
  }
  for (i = 0; i < M; i++) {
    int s, f, l;
    fscanf(fp, "%d %d %d", &s, &f, &l);
    Lmat[s][f] = l;
    Lmat[f][s] = l;
  }
  fscanf(fp, "%d", &v0);
  fclose(fp);

  for (i = 0; i < N; i++) {
    d[i] = inf;
    p[i] = -1;
  }
  d[v0] = 0;
  A[v0] = 1;
  bool loop = true;

  while (loop) {
    int num = inf;
    int pos = 0;
    for (i = 0; i < N; i++) {
      if (A[i] == 1 && d[i] < num) {
        num = d[i];
        pos = i;
      }
    }
    A[pos] = 0;

    for (i = 0; i < N; i++) {
      if (Lmat[pos][i] != 0) {
        if (d[i] == inf) {
          d[i] = d[pos] + Lmat[pos][i];
          p[i] = pos;
          A[i] = 1;
        } else if (d[i] > d[pos] + Lmat[pos][i]) {
          d[i] = d[pos] + Lmat[pos][i];
          p[i] = pos;
        }
      }
    }
    loop = false;
    for (i = 0; i < N; i++)
      if (A[i] == 1) loop = true;
  }

  for (u = 0; u < N; u++) {
    if (d[u] == inf)
      printf("vertex %d: d[%d]=inf, p[%d]=%d\n", u, u, u, p[u]);
    else
      printf("vertex %d: d[%d]=%d, p[%d]=%d\n", u, u, d[u], u, p[u]);
  }
  return 0;
}