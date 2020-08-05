//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define maxN 50
#define maxM 500
#define SWAP(a, b) (a += b, b = a - b, a -= b)

struct edge_data {
  int end1;
  int end2;
  int weight;
};

void make_set(int *p, int *rank, int x);
int find_set(int *p, int *rank, int x);
void _link(int *p, int *rank, int x, int y);
int set_union(int *p, int *rank, int x, int y);
void quicksort(int *A, int *ednum, int p, int r);
int partition(int *A, int *ednum, int p, int r);
void kruskal(int n, int m, struct edge_data *edge, int *ednum, int *T);

int main() {
  int N, M;
  struct edge_data edge[maxM];
  int W[maxM];
  int T[maxM];
  int ednum[maxM];
  int u, v, w;
  char fname[128];
  int i;
  FILE *fp;

  printf("input filename: ");
  fgets(fname, sizeof(fname), stdin);
  fname[strlen(fname) - 1] = '\0';
  fflush(stdin);
  fp = fopen(fname, "r");
  fscanf(fp, "%d %d", &N, &M);
  if (N > maxN) {
    printf("N is too large, setting N = 50\n");
    N = maxN;
  }
  for (i = 0; i < M; i++) {
    fscanf(fp, " %d %d %d", &u, &v, &w);
    edge[i].end1 = u;
    edge[i].end2 = v;
    edge[i].weight = w;
    W[i] = edge[i].weight;
    ednum[i] = i;
  }
  fclose(fp);

  for (i = 0; i < M; i++) T[i] = 0;
  quicksort(W, ednum, 0, M - 1);
  kruskal(N, M, edge, ednum, T);

  for (i = 0; i < M; i++) {
    if (T[i] == 1) {
      printf("edge (%2d, %2d)", edge[i].end1, edge[i].end2);
      printf(" weight = %d\n", edge[i].weight);
    }
  }
  return 0;
}

void make_set(int *p, int *rank, int x) {
  p[x] = x;
  rank[x] = 0;
}

int find_set(int *p, int *rank, int x) {
  int y = x;
  while (y != p[y]) y = p[y];
  int r = y;
  y = x;
  while (y != r) {
    int z = p[y];
    p[y] = r;
    y = z;
  }
  return r;
}

void _link(int *p, int *rank, int x, int y) {
  if (rank[x] > rank[y])
    p[y] = x;
  else {
    p[x] = y;
    if (rank[x] == rank[y]) rank[y] += 1;
  }
}

int set_union(int *p, int *rank, int x, int y) {
  int u = find_set(p, rank, x);
  int v = find_set(p, rank, y);
  if (u != v) {
    _link(p, rank, u, v);
    return 1;
  }
  return 0;
}

void quicksort(int *A, int *ednum, int p, int r) {
  if (p < r) {
    int q = partition(A, ednum, p, r);
    quicksort(A, ednum, p, q);
    quicksort(A, ednum, q + 1, r);
  }
}

int partition(int *A, int *ednum, int p, int r) {
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
    } else if (i < j && A[j] <= x && A[i] >= x) {
      SWAP(A[i], A[j]);
      SWAP(ednum[i], ednum[j]);
      j -= 1;
      i += 1;
    } else if (i >= j)
      break;
  }
  return j;
}

void kruskal(int n, int m, struct edge_data *edge, int *ednum, int *T) {
  int i, u;
  int p[maxN], rank[maxN];
  for (u = 0; u < n; u++) make_set(p, rank, u);
  for (i = 0; i < m; i++) {
    int k = ednum[i];
    int x = edge[k].end1;
    int y = edge[k].end2;
    T[k] = set_union(p, rank, x, y);
  }
  //必要か分からないから書いておいた。
  //必要なさそうだからmain関数に入れなかった。
  //変数のスコープは狭ければ狭いほどいいから、中身を表示するためだけにグローバル変数にしたり引数として与えたりするのは合理的でないためここで表示した。
  for (u = 0; u < n; u++) {
    printf("vertex %2d, parent=%2d rank=%2d\n", u, p[u], rank[u]);
  }
}