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

struct edge_data /* 構造体 edge_data の定義 */
{
  int end1;
  int end2;
};

void make_set(int *p, int *rank, int x);
int find_set(int *p, int *rank, int x);
void _link(int *p, int *rank, int x, int y);
void set_union(int *p, int *rank, int x, int y);

int main() {
  int N, M;
  struct edge_data edge[maxM];
  int p[maxN], rank[maxN];
  int component[maxN];
  int number_comp = 0;
  int u, v;
  char fname[128];
  int i;
  FILE *fp;

  printf("input filename: ");
  fgets(fname, sizeof(fname), stdin);
  fname[strlen(fname) - 1] = '\0';
  fflush(stdin);
  fp = fopen(fname, "r");
  fscanf(fp, "%d %d", &N, &M);
  for (i = 0; i < M; i++) {
    fscanf(fp, " %d %d", &u, &v);
    edge[i].end1 = u;
    edge[i].end2 = v;
  }
  fclose(fp);

  for (u = 0; u < N; u++) {
    make_set(p, rank, u);
    component[u] = 0;
  }
  for (i = 0; i < M; i++) {
    int x = edge[i].end1;
    int y = edge[i].end2;
    set_union(p, rank, x, y);
  }
  for (u = 0; u < N; u++) {
    int x = find_set(p, rank, u);
    if (component[x] == 0) {
      number_comp += 1;
      component[x] = number_comp;
    }
    component[u] = component[x];
  }

  printf("number of components = %d\n", number_comp);
  for (u = 0; u < N; u++) {
    printf("vertex%d component=%2d parent=%2d rank=%2d\n", u, component[u],
           p[u], rank[u]);
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

void set_union(int *p, int *rank, int x, int y) {
  int u = find_set(p, rank, x);
  int v = find_set(p, rank, y);
  if (u != v) _link(p, rank, u, v);
}