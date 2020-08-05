#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* 文字列関数を扱えるようにする */
/* 関数 parent, heapsort の宣言，他の関数も宣言すること */
int parent(int i);
int left(int i);
int right(int i);
void heapSort(int *H, int *A, int n);
void buildHeap(int *H, int *A, int n);
void insert(int *H, int i, int a);
void upHeapSort(int *H, int i);
void downHeapSort(int *H, int i);
int main(void) {
  int i;
  int Data[50]; /* 数値を格納する配列， 50 まで */
  int Heap[50]; /* ヒープを表す補助配列 */
  int N;        /* N やループで用いる int 型変数 */
  char fname[128];
  FILE *fp; /* ファイル名の変数など必要なものを宣言 */
  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
  fp = fopen(fname, "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d", &N);   /* N をファイルから読み込む */
  if (N > 50) {
    printf("N is too large, setting N = 50\n");
    N = 50; /* N が 50 を超えるときは警告をした上で */
  }         /* N =50 に設定する */
            /* ファイル名を取得して開き， */
  for (i = 0; i < N; i++) {
    fscanf(fp, "%d", &Data[i]);
  }
  fclose(fp); /* 開いたファイルを閉じる */
  for (i = 0; i < N; i++) {
    printf("%d ", Data[i]); /* ソート前の数値の出力 */
  }
  printf("\n");
  heapSort(Heap, Data, N); /* ヒープソートを呼ぶ */
  for (i = 0; i < N; i++) {
    printf("%d ", Data[i]); /* ソート後の数値の出力 */
  }
  printf("\n");
  return 0;
}
int parent(int i) { return (i - 1) / 2; }
int left(int i) { return 2 * i + 1; }
int right(int i) { return 2 * i + 2; }
void heapSort(int *H, int *A, int n) {
  int i;
  buildHeap(H, A, n);
  int j = 0;
  printf("build-heap\n");
  for (j = 0; j < n; j++) {
    printf("%d ", H[j]); /* ソート後の数値の出力 */
  }
  printf("\n取り出し\n");
  for (i = 1; i < n; i++) {
    A[n - i] = H[0];
    H[0] = H[n - i];
    downHeapSort(H, n - i - 1);
    printf("%d回目\n", i);
    for (j = 0; j < n - i; j++) {
      printf("%d ", H[j]); /* ソート後の数値の出力 */
    }
    printf("\n");
  }
  A[0] = H[0];
}
void buildHeap(int *H, int *A, int n) {
  int i = 0;
  for (i = 0; i < n; i++) {
    insert(H, i, A[i]);
  }
}
void insert(int *H, int i, int a) {
  H[i] = a;
  upHeapSort(H, i);
}
void upHeapSort(int *H, int i) {
  int u = i;
  while (u > 0 && H[parent(u)] < H[u]) {
    int tmp = H[parent(u)];
    H[parent(u)] = H[u];
    H[u] = tmp;
    u = parent(u);
  }
}
void downHeapSort(int *H, int i) {
  int u = 0;
  while (true) {
    int l = left(u) <= i ? left(u) : u;
    int r = right(u) <= i ? right(u) : u;
    if (H[l] > H[u] && H[l] >= H[r]) {
      int tmp = H[u];
      H[u] = H[l];
      H[l] = tmp;
      u = l;
    } else if (H[r] > H[u] && H[l] <= H[r]) {
      int tmp = H[u];
      H[u] = H[r];
      H[r] = tmp;
      u = r;
    } else
      break;
  }
}