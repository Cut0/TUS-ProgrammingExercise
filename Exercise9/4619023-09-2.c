
//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void mergeSort(int *A, int p, int r);
void merge(int *A, int p, int q, int r);
int main(void) {
  int Data[50]; /* 入力数値の配列，数値数は 50 まで */
  int N;
  int i, j;

  char fname[128];                    /* 読み込むファイルの名前 */
  FILE *fp;                           /* 入力ファイル */
  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
  fp = fopen(fname, "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d", &N);
  if (N > 50) {
    printf("N is too large, setting N = 50\n");
    N = 50;
  }
  for (i = 0; i < N; i++) { /* 整数をファイルから読み込む */
    fscanf(fp, "%d", &Data[i]);
  }
  fclose(fp); /* 開いたファイルを閉じる */
  printf("読み込まれたデータ: ");
  for (i = 0; i < N; i++) {
    printf("[%d]", Data[i]);
  }
  printf("\n");
  mergeSort(Data, 0, N - 1);
  printf("ソートされたデータ: ");
  for (i = 0; i < N; i++) {
    printf("[%d]", Data[i]);
  }
  return 0;
}

void mergeSort(int *A, int p, int r) {
  if (p < r) {
    int q = (p + r) / 2;
    mergeSort(A, p, q);
    mergeSort(A, q + 1, r);
    merge(A, p, q, r);
  }
}
void merge(int *A, int p, int q, int r) {
  int i = p, j = q + 1, k = p, u = 0;
  int B[r + 1];
  for (u = 0; u < r + 1; u++) {
    B[u] = 0;
  }
  while (true) {
    if (A[i] <= A[j]) {
      B[k] = A[i];
      i += 1;
    } else {
      B[k] = A[j];
      j += 1;
    }
    k += 1;
    if (i > q || j > r) break;
  }
  int count = 0;
  if (i <= q) {
    for (; i <= q; i++) {
      B[k + count] = A[i];
      count += 1;
    }
  }
  if (j <= r) {
    for (; j <= r; j++) {
      B[k + count] = A[j];
      count += 1;
    }
  }
  for (i = p; i <= r; i++) A[i] = B[i];
}