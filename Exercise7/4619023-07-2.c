//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct cell {
  int key;
  int next; /* 構造体 cell の定義 */
};

void radix_sort_bucket(int *A, int n, int k, int d);
/* 関数 radix_sort_bucket の宣言 */
int main(void) {
  int Data[50]; /* 入力数値の配列，数値数は 50 まで */
  int N, k, d;  /* 数値の数 N，k 進数，桁数 d */
  int i, j;

  char fname[128];            /* 読み込むファイルの名前 */
  FILE *fp;                   /* 入力ファイル */
  printf("input filename: "); /* ファイルを開，入力を読み込み */
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
  radix_sort_bucket(Data, N, k, d); /* radix_sort を呼び出し */
  printf("ソートされたデータ: ");
  for (i = 0; i < N; i++) {
    printf("[%d]", Data[i]);
  }
  return 0;
}
void radix_sort_bucket(int *A, int n, int k, int d) {
  struct cell List[50];
  int B[50];
  int p, i, j;
  int pos;

  for (p = 0; p < d; p++) {
    pos = 0;
    for (i = 0; i < n; i++) {
      List[i].key = A[i];
      List[i].next = -1;
    }
    for (i = 0; i < k; i++) {
      B[i] = -1;
    }
    for (j = n - 1; j >= 0; j--) {
      i = (int)(List[j].key / pow(k, p)) % k;
      List[j].next = B[i];
      B[i] = j;
    }
    for (j = 0; j < k; j++) {
      int now = B[j];
      while (now != -1) {
        A[pos] = List[now].key;
        now = List[now].next;
        pos += 1;
      }
    }
  }
}