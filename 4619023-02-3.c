//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void printData(int N, int *data) {
  int sum = 0, i;
  for (i = 0; i < N; i++) {
    printf("%d人目:", i);
    printf("%d点\n", data[i]);
    sum += data[i];
  }
  double res = (double)sum / N;
  printf("平均点:%f", res);
}
int main() {
  int N, i;
  int *data;
  printf("人数を入力:");
  scanf("%d", &N);
  data = (int *)malloc(N * sizeof(int));
  for (i = 0; i < N; i++) {
    int num;
    printf("%d人目の点数を入力:", i + 1);
    scanf("%d", &num);
    data[i] = num;
  }
  printData(N, data);

  return 0;
}