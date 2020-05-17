//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int i = 0;
void doubleArray(double* array, int N) {
  for (i = 0; i < N; i++) {
    array[i] = array[i] * array[i];
  }
}
void printArray(double* array, int N) {
  for (i = 0; i < N; i++) {
    printf("%dつ目の要素:", i + 1);
    printf("%lf\n", array[i]);
  }
}
int main() {
  int i = 0, n = 0;
  double tmp = 0;
  double* test;
  printf("要素数(N)を入力:");
  scanf("%d", &n);
  test = (double*)malloc(n * sizeof(double));
  for (i = 0; i < n; i++) {
    printf("%dつ目の要素:", i + 1);
    scanf("%lf", &tmp);
    test[i] = tmp;
  }
  doubleArray(test, n);
  printf("結果表示\n");
  printArray(test, n);
  free(test);
  return 0;
}