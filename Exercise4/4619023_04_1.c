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
void printParams(struct node *T);
void inorder_tree_walk(struct node *T, struct node *x);
struct node *tree_search(struct node *T, struct node *x, int a);
struct node *tree_minimum(struct node *T, struct node *x);
struct node *tree_maximum(struct node *T, struct node *x);
struct node *tree_insert(struct node *T, struct node *x);
struct node *tree_delete(struct node *T, struct node *z);

int main(void) {
  struct node *Tree; /* ２分探索木を表す変数は Tree */
  struct node *x;    /* 挿入，削除等に用いる node 型変数 */
  int N;             /* 数値の数は N */
  int Data[50];      /* 数値を格納する配列は Data */
  int i;
  char fname[128]; /* 読み込むファイルの名前を格納する変数 */
  FILE *fp; /* ファイル名は 128 文字まで対応可能にする */
  printf("input filename: ");         /* ファイル名の入力を要求 */
  fgets(fname, sizeof(fname), stdin); /* 標準入力からファイル名を取得 */
  fname[strlen(fname) - 1] = '\0'; /* 最後の改行コードを除去 */
  fflush(stdin); /* 128文字を超えた入力を標準入力から捨てる */
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

  for (i = 0; i < N; i++) {
    x = (struct node *)malloc(sizeof(struct node));
    x->key = Data[i]; /* 新しい頂点 x を生成して key などを指定 */
    x->parent = x->left = x->right = NULL;
    Tree = tree_insert(Tree, x);
    printParams(Tree);
  }
  free(x);
  for (i = 0; i < N; i++) {
    x = (struct node *)malloc(sizeof(struct node));
    x = tree_search(Tree, Tree, Data[i]);
    Tree = tree_delete(Tree, x);
    printParams(Tree);
  }
  free(Tree);
  return 0;
}

void printParams(struct node *T) {
  if (T != NULL) {
    printf("keys in tree:");
    inorder_tree_walk(T, T);
    printf(" Min:");
    printf("%d", tree_minimum(T, T)->key);
    printf(" Max:");
    printf("%d\n", tree_maximum(T, T)->key);
  } else {
    printf("keys in tree: NULL");
    printf(" Min:");
    printf(" NULL");
    printf(" Max:");
    printf(" NULL\n");
  }
}

void inorder_tree_walk(struct node *T, struct node *x) {
  if (x != NULL) {
    inorder_tree_walk(T, x->left);
    printf(" %d,", x->key);
    inorder_tree_walk(T, x->right);
  }
}

struct node *tree_search(struct node *T, struct node *x, int a) {
  while (x != NULL && x->key != a) {
    if (a < x->key)
      x = x->left;
    else
      x = x->right;
  }
  return x;
}

struct node *tree_minimum(struct node *T, struct node *x) {
  struct node *y = NULL;
  while (x != NULL) {
    y = x;
    x = x->left;
  }
  return y;
}

struct node *tree_maximum(struct node *T, struct node *x) {
  struct node *y = NULL;
  while (x != NULL) {
    y = x;
    x = x->right;
  }
  return y;
}

/*以下，関数 tree insert の冒頭部分：*/
struct node *tree_insert(struct node *T, struct node *x) {
  struct node *r, *y, *z;
  r = T; /* 根を変数 r に格納 */
  y = NULL;
  z = r;
  char lastPos = 's';
  while (z != NULL) {
    if (x->key < z->key) {
      y = z;
      z = z->left;
      lastPos = 'l';
    } else {
      y = z;
      z = z->right;
      lastPos = 'r';
    }
  }
  x->parent = y;
  if (y == NULL)
    r = x;
  else {
    if (lastPos == 'l')
      y->left = x;
    else if (lastPos == 'r')
      y->right = x;
  }
  return r;
}

struct node *tree_delete(struct node *T, struct node *z) {
  struct node *r, *y, *p, *x;
  r = T;
  if (z->right == NULL || z->left == NULL)
    y = z;
  else
    y = tree_minimum(T, z->right);
  p = y->parent;
  if (y->right != NULL) {
    x = y->right;
    x->parent = p;
  } else if (y->left != NULL) {
    x = y->left;
    x->parent = p;
  } else
    x = NULL;
  if (p == NULL)
    r = x;
  else {
    if (y == p->left)
      p->left = x;
    else
      p->right = x;
  }
  if (y != z) z->key = y->key;
  free(y);
  return r;
}