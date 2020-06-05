#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* 文字列関数を扱えるようにする */
/* 関数 parent, heapsort の宣言，他の関数も宣言すること */
int parent(int i);
void heapSort(int *a, int length);
void downheap(int k, int r, int *a);
void swap(int i, int j, int *a);
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
  heapSort(Data, N); /* ヒープソートを呼ぶ */
  for (i = 0; i < N; i++) {
    printf("%d ", Data[i]); /* ソート後の数値の出力 */
  }
  printf("\n");
}
void heapSort(int *a, int length) {  // ヒープソート(昇順)
  for (int i = (length - 2) / 2; i >= 0; i--) {
    downheap(i, length - 1, a);
  }
  for (int i = length - 1; i > 0; i--) {
    swap(0, i, a);
    downheap(0, i - 1, a);
  }
}

void downheap(int k, int r, int *a) {
  int j, v;
  v = a[k];
  while (true) {
    j = 2 * k + 1;
    if (j > r) break;
    if (j != r) {
      if (a[j + 1] > a[j]) {
        j = j + 1;
      }
    }
    if (v >= a[j]) break;
    a[k] = a[j];
    k = j;
  }
  a[k] = v;
}

void swap(int i, int j, int *a) {  // 要素の交換
  int tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}