#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* 文字列関数を扱えるようにする */
struct node         /* 構造体 node の定義 */
{
  int key;
  struct node *parent;
  struct node *left;
  struct node *right;
};
/* 関数 tree_search, tree_minimum, tree_insert の宣言 */
struct node *tree_search(struct node *T, int a);
struct node *tree_minimum(struct node *x);
struct node *tree_insert(struct node *T, struct node *x);
/* （他の関数も宣言すること） */
main(void) {
  struct node *Tree;      /* ２分探索木を表す変数は Tree */
  struct node *x, *y, *z; /* 挿入，削除等に用いる node 型変数 */
  int N;                  /* 数値の数は N */
  int Data[50];           /* 数値を格納する配列は Data */
  int i;
  int a;
  char fname[128]; /* 読み込むファイルの名前を格納する変数 */
  FILE *fp; /* ファイル名は 128 文字まで対応可能にする */
  fp = fopen(fname, "r"); /* ファイルを読み込みモードで開く */
  fscanf(fp, "%d", &N);   /* N をファイルから読み込む */
  if (N > 50) {
    printf("N is too large, setting N = 50\n");
    N = 50; /* N が 50 を超えるときは警告をした上で */
  }         /* N =50 に設定する */
  for (i = 0; i < N; i++) {
    fscanf(fp, "%d", &Data[i]); /* 整数値を順に読み込み，Data に格納する */
  }
  fclose(fp);  /* 開いたファイルを閉じる */
  Tree = NULL; /* Tree の初期化，最初は空 */
  /* Tree は２分探索木の根を指す*/
  for (i = 0; i < N; i++) { /* データ挿入部分のくりかえし
       x = (struct node *)malloc(size of (struct node));
       x->key = Data[i]; /* 新しい頂点 x を生成して key などを指定 */
    x->parent = x->left = x->right = NULL;
    Tree = tree_insert(Tree, x); /* x を Tree に挿入して根の情報を更新*/
                                 /* ループ主要部分（各自作成） */
  }
  for (i = 0; i < N; i++) { /* データ削除部分のくりかえし */
                            /* ループ主要部分（各自作成） */
  }
}
/*以下，関数 tree insert の冒頭部分：*/
struct node *tree_insert(struct node *T, struct node *x) {
  struct node *r, *y, *z;
  r = T; /* 根を変数 r に格納 */
  y = NULL;
  z = r; /* 主要部分（各自作成） */
  return r;
}