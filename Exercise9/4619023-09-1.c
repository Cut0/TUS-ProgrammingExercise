//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void quicksort(int *A, int p, int r);
int partition(int *A, int, int);
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
  quicksort(Data, 0, N - 1);
  printf("ソートされたデータ: ");
  for (i = 0; i < N; i++) {
    printf("[%d]", Data[i]);
  }
  return 0;
}

void quicksort(int *A, int p, int r) {
  if (p < r) {
    int q = partition(A, p, r);
    quicksort(A, p, q);
    quicksort(A, q + 1, r);
  }
}
int partition(int *A, int p, int r) {
  int x = A[p];
  int i = p;
  int j = r;
  while (true) {
    if (A[j] > x) {
      j -= 1;
      continue;
    } else if (A[i] < x) {
      i += 1;
      continue;
    }
    if (i < j && A[j] <= x && A[i] >= x) {
      int tmp = A[i];
      A[i] = A[j];
      A[j] = tmp;
      j -= 1;
      i += 1;
    }
    if (i >= j) break;
  }
  return j;
}