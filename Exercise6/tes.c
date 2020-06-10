// 内部ハッシュ法
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // 文字列関数を扱えるようにする

#define W 10  // W = 文字列の最大長さ，ここでは 10 に設定
#define m 97  // m = ハッシュ表のサイズ，ここでは 97 に設定
#define n 50  // n = 扱う文字列の最大数，ここでは 50 に設定
#define EMPTY 0
#define OCCUPIED 1
#define DELETED 2

struct cell {
  char key[W + 1];
  int next;
  unsigned int state : 2;  //構造体 cell の定義
};

int hash_val(char *a);
int hash_search(struct cell *A, char *a);  // 関数 hash_search の宣言
int hash_val(char *a);
void hash_insert(struct cell *A, char *a);
void hash_delete(struct cell *A, char *a);

int main(void) {
  struct cell A[m];  // ハッシュ表を表すの配列
  int N;             // 数値の数は N
  char word[W + 1];  // ファイルから読み込んだ文字列を格納する変数
  int head, freeL;  // リスト List の先頭，空きアドレスリストの先頭
  int i, j, h, x, y, count;
  char fname[128];  // 読み込むファイルの名前
  FILE *fp;         // 入力ファイル

  for (h = 0; h < m; h++) {  // ハッシュ表のの初期化
    A[h].state = 0;
  }

  printf("input filename: ");          // ファイル名の入力を要求
  fgets(fname, sizeof(fname), stdin);  // 標準入力からファイル名を取得
  fname[strlen(fname) - 1] = '\0';  // 最後の改行コードを除去
  fflush(stdin);  // 128文字を超えた入力を標準入力から捨てる
  fp = fopen(fname, "r");  // ファイルを読み込みモードで開く
  fscanf(fp, "%d", &N);    // N をファイルから読み込む

  for (i = 0; i < N; i++) {
    fscanf(fp, "%s", word);  // 文字列をファイルから読み込み，wordに格納
    x = hash_search(A, word);  // ハッシュ表の中で文字列 word を探索
    if (x == -1) {
      hash_insert(A, word);
    }
  }
  fclose(fp);  // 開いたファイルを閉じる

  for (i = 0; i < m; i++) {
    if (A[i].state == OCCUPIED) {
      printf("[%d]: %s", i, A[i].key);
      printf("\n");
    }
  }
  printf("\n");

  fp = fopen(fname, "r");  // ファイルを読み込みモードで開く
  fscanf(fp, "%d", &N);    // N をファイルから読み込む

  for (i = 0; i < N; i++) {
    fscanf(fp, "%s", word);  // 文字列をファイルから読み込み，wordに格納
    x = hash_search(A, word);  // ハッシュ表の中で文字列 word を探索
    if (x != -1) {
      printf("Delete: %s\n", A[x].key);
      hash_delete(A, A[x].key);
    }
  }
  fclose(fp);  // 開いたファイルを閉じる

  for (j = 0; j < m; j++) {
    if (A[j].state == OCCUPIED) {
      printf("[%d]: %s", j, A[j].key);
      printf("\n");
    }
  }

  printf("\nEND\n");

  return 0;
}

int hash_val(char *a)  // 文字列はポインタ渡し
{
  int h, i;
  h = 0;
  i = 0;
  while (a[i] != 0 && i < W) {  // 文字の整数コードの和を計算
    h = h + (int)a[i];
    i = i + 1;
  }
  h = h % m;  // m で割った余りを取る
  return h;
}

void hash_insert(struct cell *A, char *a) {
  int h = hash_val(a);
  int x = -1;
  int i = 0;

  while (x == -1 && i < m) {
    if (A[(h + i) % m].state != OCCUPIED) {
      x = (h + i) % m;
    } else {
      i += 1;
    }
  }
  if (x == -1) {
    printf("error: out of space\n");
  } else {
    strcpy(A[x].key, a);
    A[x].state = OCCUPIED;
  }
}

int hash_search(struct cell *A, char *a) {
  int h = hash_val(a);
  int x = -1;
  int i = 0;
  int do_loop = 1;
  unsigned int s;

  while (do_loop == 1) {
    s = A[(h + i) % m].state;
    if (s == OCCUPIED && strcmp(A[(h + i) % m].key, a) == 0) {
      x = (h + i) % m;
      do_loop = 0;
    } else {
      if (s == EMPTY) {
        do_loop = 0;
      } else {
        i += 1;
        if (i >= m) {
          do_loop = 0;
        }
      }
    }
  }
  return x;
}

void hash_delete(struct cell *A, char *a) {
  int x;

  x = hash_search(A, a);
  if (x != -1) {
    *A[x].key = ' ';
    A[x].state = DELETED;
  }
}
