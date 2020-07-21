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

struct cell /* 構造体 cell の定義 */
{
  int key;
  int vertex;
};
int parent(int i);
int left(int i);
int right(int i);
void insert(struct cell *H, int *adr, int i, int a, int v);
void decrease_key(struct cell *H, int *adr, int i, int a);
int delete_min(struct cell *H, int *adr, int i);
void upHeapSort(struct cell *H, int *adr, int i);
void downHeapSort(struct cell *H, int *adr, int i);

void swap(int *a, int *b) {
  int tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

int hsize; /* ヒープに格納された頂点の数 */
int main() {
  struct cell Heap[maxN]; /* ヒープに用いる配列の宣言 */
  int adr[maxN]; /* ヒープに含まれる頂点のアドレスの配列 */

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
    for (j = 0; j < maxN; j++) Lmat[i][j] = inf;
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

  for (u = 0; u < N; u++) { /* p[u], d[u], adr[u] の初期化 */
    d[u] = inf;
    p[u] = -1;
    adr[u] = -1;
  }

  d[v0] = 0;
  A[v0] = 1;
  hsize = 1;
  insert(Heap, adr, hsize, d[v0], v0);
  while (hsize > 0) {
    v = delete_min(Heap, adr, hsize);
    hsize -= 1;
    for (j = 0; j < N; j++) {
      if (Lmat[v][j] != inf) {
        if (d[j] == inf) {
          d[j] = d[v] + Lmat[v][j];
          p[j] = v;
          hsize += 1;
          insert(Heap, adr, hsize, d[j], j);
        } else if (d[j] > d[v] + Lmat[v][j]) {
          d[j] = d[v] + Lmat[v][j];
          p[j] = v;
          decrease_key(Heap, adr, adr[j], d[j]);
        }
      }
    }
  }
  for (u = 0; u < N; u++) {
    if (d[u] == inf)
      printf("vertex %d: d[%d]=inf, p[%d]=%d\n", u, u, u, p[u]);
    else
      printf("vertex %d: d[%d]=%d, p[%d]=%d\n", u, u, d[u], u, p[u]);
  }
  return 0;
}

int parent(int i) { return (i - 1) / 2; }
int left(int i) { return 2 * i + 1; }
int right(int i) { return 2 * i + 2; }
void insert(struct cell *H, int *adr, int i, int a, int v) {
  H[i - 1].key = a;
  H[i - 1].vertex = v;
  adr[v] = i - 1;
  upHeapSort(H, adr, i - 1);
}

void decrease_key(struct cell *H, int *adr, int i, int a) {
  H[i].key = a;
  upHeapSort(H, adr, i);
}

int delete_min(struct cell *H, int *adr, int i) {
  int a = H[0].key;
  int v = H[0].vertex;
  adr[v] = -1;
  if (hsize > 1) {
    H[0].key = H[i - 1].key;
    H[0].vertex = H[i - 1].vertex;
    adr[H[0].vertex] = 0;
    downHeapSort(H, adr, i - 2);
  }
  return v;
}

void upHeapSort(struct cell *H, int *adr, int i) {
  int u = i;
  while (u > 0 && H[parent(u)].key > H[u].key) {
    swap(&H[u].key, &H[parent(u)].key);
    swap(&H[u].vertex, &H[parent(u)].vertex);
    swap(&adr[H[u].vertex], &adr[H[parent(u)].vertex]);
    u = parent(u);
  }
}

void downHeapSort(struct cell *H, int *adr, int i) {
  int u = 0;
  while (true) {
    int l = left(u) <= i ? left(u) : u;
    int r = right(u) <= i ? right(u) : u;
    if (H[l].key < H[u].key && H[l].key <= H[r].key) {
      swap(&H[u].key, &H[parent(u)].key);
      swap(&H[u].vertex, &H[parent(u)].vertex);
      swap(&adr[H[u].vertex], &adr[H[parent(u)].vertex]);
      u = l;
    } else if (H[r].key < H[u].key && H[l].key >= H[r].key) {
      swap(&H[u].key, &H[parent(u)].key);
      swap(&H[u].vertex, &H[parent(u)].vertex);
      swap(&adr[H[u].vertex], &adr[H[parent(u)].vertex]);
      u = r;
    } else
      break;
  }
}