//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void counting_radix_sort(int *A, int n, int k, int d);
/* 関数 bucket_sort の宣言 */
int main(void) {
  int Data[50]; /* 入力数値の配列，数値数は 50 まで */
  int N, k, d;
  int i, j;

  char fname[128];                    /* 読み込むファイルの名前 */
  FILE *fp;                           /* 入力ファイル */
  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
  fp = fopen(fname, "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d %d %d", &N, &k, &d);
  /* N, l, u, m をファイルから読み込む */
  if (N > 50) {
    printf("N is too large, setting N = 50\n");
    N = 50;
  }
  for (i = 0; i < N; i++) { /* 整数をファイルから読み込む */
    int num;
    fscanf(fp, "%d", &num);
    if (num < 0) {
      printf("%dは負の数であるため除外\n", num);
      i--;
      N--;
    } else if (num >= pow(k, d)) {
      printf("%dは%d進数表現において%d桁を超えているため除外\n", num, k, d);
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
  counting_radix_sort(Data, N, k, d);
  printf("ソートされたデータ: ");
  for (i = 0; i < N; i++) {
    printf("[%d]", Data[i]);
  }
  return 0;
}

void counting_radix_sort(int *A, int n, int k, int d) {
  int i = 0, p = 0, j = 0;
  int c[k];
  int b[n];
  for (i = 0; i < n; i++) b[i] = 0;
  for (p = 0; p < d; p++) {
    for (i = 0; i < k; i++) c[i] = 0;
    for (i = 0; i < n; i++) {
      int num = (int)(A[i] / pow(k, p)) % k;
      c[num] += 1;
    }
    for (i = 1; i < k; i++) c[i] += c[i - 1];
    for (j = n - 1; j >= 0; j--) {
      int num = (int)(A[j] / pow(k, p)) % k;
      b[c[num] - 1] = A[j];
      c[num] -= 1;
    }
    for (j = 0; j < n; j++) A[j] = b[j];
  }
}
