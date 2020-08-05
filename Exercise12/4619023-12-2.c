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

void quicksort(int *A, int *ednum, int p, int r);
int partition(int *A, int *ednum, int p, int r);

int main() {
  int N, M;
  struct edge_data edge[maxM];
  int W[maxM];
  int ednum[maxM];
  int u, v, w;
  char fname[128];
  int i, k;
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
  }
  fclose(fp);

  for (i = 0; i < M; i++) {
    W[i] = edge[i].weight;
    ednum[i] = i;
  }

  quicksort(W, ednum, 0, M - 1);
  for (i = 0; i < M; i++) {
    k = ednum[i];
    printf("edge %2d: (%2d, %2d)", k, edge[k].end1, edge[k].end2);
    printf(" weight = %d\n", edge[k].weight);
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