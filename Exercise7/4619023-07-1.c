//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
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
void bucket_sort(int *A, int n, int l, int u, int m);
/* 関数 bucket_sort の宣言 */
int main(void) {
  int Data[50]; /* 入力数値の配列，数値数は 50 まで */
  int N, l, u, m; /* 数値の数 N，値の下界 l, 上界 u, バケット数 m */
  int i, j;

  char fname[128];                    /* 読み込むファイルの名前 */
  FILE *fp;                           /* 入力ファイル */
  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
  fp = fopen(fname, "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d %d %d %d", &N, &l, &u, &m);
  /* N, l, u, m をファイルから読み込む */
  if (N > 50) {
    printf("N is too large, setting N = 50\n");
    N = 50;
  }
  for (i = 0; i < N; i++) { /* 整数をファイルから読み込む */
    int num;
    fscanf(fp, "%d", &num);
    if (num < l || num > u) {
      printf("%dは%d~%dの範囲外なので除外\n", num, l, u);
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
  bucket_sort(Data, N, l, u, m); /* bucket_sort を 呼び出し */
  printf("ソートされたデータ: ");
  for (i = 0; i < N; i++) {
    printf("[%d]", Data[i]);
  }
  return 0;
}

void bucket_sort(int *A, int n, int l, int u, int m) {
  struct cell List[50]; /* リストを格納する配列 */
  int B[50];            /* バケットを表す配列 */
  int i, j;
  int pos = 0;
  int T[m + 1];
  T[0] = 0;
  for (i = 0; i < n; i++) { /* あらかじめ n 個の数値をリストに格納 */
    List[i].key = A[i]; /* next の値を初期化 */
    List[i].next = -1;
  }
  for (j = 0; j < m; j++) { /* バケットを初期化 */
    B[j] = -1;
  }
  for (j = 0; j <= m; j++) { /* Tを初期化 */
    T[j] = -1;
  }
  //作成
  for (i = 0; i < n; i++) {
    int pos = (m * A[i]) / (u - l);
    if (B[pos] >= 0 && B[pos] <= n) {
      List[i].next = B[pos];
    }
    B[pos] = i;
  }
  //ソート
  for (i = 0; i < m; i++) {
    if (B[i] != -1) {
      A[pos] = List[B[i]].key;
      int next = List[B[i]].next;
      while (next != -1) {
        pos += 1;
        A[pos] = List[next].key;
        next = List[next].next;
      }
      pos += 1;
    }
    T[i + 1] = pos;
  }

  for (j = 0; j < m; j++) {
    for (i = 1 + T[j]; i < T[j + 1]; i++) {
      int a = A[i];
      int h = i - 1;
      while (h >= 0 && A[h] > a) {
        A[h + 1] = A[h];
        h = h - 1;
        A[h + 1] = a;
      }
    }
  }
}