#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* 文字列関数を扱えるようにする */
#define W 10 /* W = 文字列の最大長さ，ここでは 10 に設定 */
#define m 97 /* m = ハッシュ表のサイズ，ここでは 97 に設定 */
#define n 50 /* n = 扱う文字列の最大数，ここでは 50 に設定 */
struct cell {
  char key[W + 1];
  unsigned int state : 2;
};
int hash_search(struct cell *A, char *a);
void hash_insert(struct cell *A, char *a);
void hash_delete(struct cell *A, char *a);
int hash_val(char *a);
void print_data(struct cell *A, int x);
/* 関数 chained_hash_searchの宣言 */
/* （他の関数も宣言すること） */
int main(void) {
  struct cell A[m]; /* ハッシュ表を表すの配列 */
  int N;
  int h, i, x; /* 数値の数は N */
  char word[W + 1]; /* ファイルから読み込んだ文字列を格納する変数 */
  char fname[128];
  FILE *fp; /* 入力ファイル */
  for (h = 0; h < m; h++) A[h].state = 0;
  /* ハッシュ表の初期化 */
  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
  fp = fopen(fname, "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d", &N);   /* N をファイルから読み込む */
  if (N > n) {
    printf("N is too large, setting N = %d\n", n);
    N = n;
  }

  for (i = 0; i < N; i++) {
    fscanf(fp, "%s", word);  // 文字列をファイルから読み込み，wordに格納
    x = hash_search(A, word);  // ハッシュ表の中で文字列 word を探索
    if (x == -1) hash_insert(A, word);
  }
  fclose(fp); /* 開いたファイルを一旦閉じる */

  printf("BEFORE:[\n");
  for (i = 0; i < m; i++) print_data(A, i);
  printf("]\n\n");

  fp = fopen(fname, "r"); /* ファイルを再度読み込みモードで開く */
  fscanf(fp, "%d", &N); /* N をファイルから読み込む */
  if (N > n) {
    N = n;
  }
  for (i = 0; i < N; i++) {
    fscanf(fp, "%s", word);
    x = hash_search(A, word);
    if (x != -1) hash_delete(A, A[x].key);
  }
  fclose(fp);

  printf("AFTER:[\n");
  for (i = 0; i < m; i++) print_data(A, i);
  printf("]");

  return 0;
}

int hash_search(struct cell *A, char *a) {
  int i = 0;
  int x = hash_val(a);
  unsigned int state;
  for (i = 0; i <= m; i++) {
    int x = (hash_val(a) + i) % m;
    state = A[x].state;
    if (state == 1 && strcmp(A[x].key, a) == 0) return x;
    if (state == 0) return -1;
  }
  return -1;
}

void hash_insert(struct cell *A, char *a) {
  int h = hash_val(a);
  int x = -1;
  int i = 0;
  while (x == -1 && i < m) {
    if (A[(h + i) % m].state != 1) {
      x = (h + i) % m;
    } else {
      i += 1;
    }
  }
  if (x == -1) {
    printf("error: out of space\n");
  } else {
    strcpy(A[x].key, a);
    A[x].state = 1;
  }
}

void hash_delete(struct cell *A, char *a) {
  int x = hash_search(A, a);
  if (x != -1) {
    *A[x].key = '\0';
    A[x].state = 2;
  }
}

int hash_val(char *a) /* 文字列はポインタ渡し */
{
  int h, i;
  h = 0;
  i = 0;
  while (a[i] != 0 && i < W) { /* 文字の整数コードの和を計算 */
    h = h + (int)a[i];
    i = i + 1;
  }
  h = h % m; /* m で割った余りを取る */
  return h;
}

void print_data(struct cell *A, int x) {
  char *state;
  switch (A[x].state) {
    case 1:
      state = "Occupied";
      break;
    case 2:
      state = "Deleted";
      break;
  }
  if (A[x].state)
    printf(" {\n   State:'%s',\n   id:%d,\n   key:%s\n },\n", state, x,
           A[x].key);
}