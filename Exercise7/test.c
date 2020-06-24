// 4619055 辰川力駆　課題1
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* 文字列関数を扱えるようにする */

struct cell {
  int key;
  int next; /* 構造体 cell の定義 */
};

void bucket_sort(int *A, int n, int l, int u, int m);

int main(void) {
  int Data[50]; /* 入力数値の配列，数値数は 50 まで */
  int N, l, u, m; /* 数値の数 N，値の下界 l, 上界 u, バケット数 m */
  int i, j;
  /* 必要な変数を宣言 */
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
  for (i = 0; i < N; i++) {
    fscanf(fp, "%d", &Data[i]); /* 整数をファイルから読み込む */
    if (Data[i] < l || Data[i] > u) /* [l, u] の範囲内かを判定し，範囲外なら*/
    {
      printf("データが%d~%dの範囲外なので、このデータ%dは除外します。\n", l, u,
             Data[i]);
      N--;
      i--;
    }
  }
  fclose(fp); /* 開いたファイルを閉じる */
  printf("%d~%dの範囲にある、ソートする前のデータを表示します。\n", l, u);
  for (i = 0; i < N + 1; i++) {
    printf("[%d],", Data[i]);
  }
  printf("\n");

  bucket_sort(Data, N, l, u, m); /* bucket_sort を呼び出し */

  printf("%d~%dの範囲にある、ソートした後のデータを表示します。\n", l, u);
  for (i = 0; i < N; i++) {
    printf("[%d],", Data[i]);
  }
  printf("\n");

  return 0;
}

void bucket_sort(int *A, int n, int l, int u, int m) {
  struct cell List[50]; /* リストを格納する配列 */
  int B[50];            /* バケットを表す配列 */
  int i, j;
  int range = (u - l) / m;  //全て整数なのでこれで範囲ok
  int post = 0;             //どれだけAに格納したか
  int headp = 0;            // ソートする区間の先頭を保存
  int tmp;
  int now;
  /* 必要な変数を宣言 */
  for (i = 0; i < n; i++) { /* あらかじめ n 個の数値をリストに格納 */
    List[i].key = A[i]; /* next の値を初期化 */
    List[i].next = -1;
  }
  for (j = 0; j < m; j++) { /* バケットを初期化 */
    B[j] = -1;
  }

  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      if (List[j].key >= range * i && List[j].key < range * (i + 1)) {
        List[j].next = B[i];
        B[i] = j;
      }
    }
  }

  for (i = 0; i < m; i++) {
    now = B[i];
    headp = post;
    while (now != -1) {
      A[post] = List[now].key;
      for (j = post; j > headp; j--) {
        if (A[j] < A[j - 1]) {
          tmp = A[j];
          A[j] = A[j - 1];
          A[j - 1] = tmp;
        }
      }
      now = List[now].next;
      post++;
    }
  }
}