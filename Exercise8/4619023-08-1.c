//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void counting_sort(int *A, int n, int k);
/* 関数 bucket_sort の宣言 */
int main(void) {
  int Data[50]; /* 入力数値の配列，数値数は 50 まで */
  int N, k;
  int i, j;

  char fname[128];                    /* 読み込むファイルの名前 */
  FILE *fp;                           /* 入力ファイル */
  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
  fp = fopen(fname, "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d %d", &N, &k);
  /* N, l, u, m をファイルから読み込む */
  if (N > 50) {
    printf("N is too large, setting N = 50\n");
    N = 50;
  }
  for (i = 0; i < N; i++) { /* 整数をファイルから読み込む */
    int num;
    fscanf(fp, "%d", &num);
    if (num < 0 || num > k - 1) {
      printf("%dは%d~%dの範囲外なので除外\n", num, 0, k - 1);
      i--;
      N--;
    } else {
      Data[i] = num;
    }
  }
  fclose(fp); /* 開いたファイルを閉じる */
  printf("読み込まれたデータ: ");
  for (i = 0; i < N; i++) {
    printf("[%d]", Data[i]);
  }
  printf("\n");
  counting_sort(Data, N, k);
  printf("ソートされたデータ: ");
  for (i = 0; i < N; i++) {
    printf("[%d]", Data[i]);
  }
  return 0;
}

void counting_sort(int *A, int n, int k) {
  int i = 0;
  int c[k];
  int b[n];
  for (i = 0; i < n; i++) b[i] = 0;
  for (i = 0; i < k; i++) c[i] = 0;
  for (i = 0; i < n; i++) c[A[i]] += 1;
  for (i = 1; i < k; i++) c[i] += c[i - 1];
  for (i = n - 1; i >= 0; i--) {
    b[c[A[i]] - 1] = A[i];
    c[A[i]] = c[A[i]] - 1;
    printf("[%d]", c[A[i]] - 1);
  }
  for (i = 0; i < n; i++) A[i] = b[i];
}