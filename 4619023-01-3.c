//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

double limitfunc(double a, double x, int count) {
  if (count < 0) {
    printf("収束しませんでした");
    exit(0);
    return 0;
  }
  double nextValue = a * x * x + 1;
  if (fabs(x - nextValue) <= 0.000001) {
    return x;
  }
  return limitfunc(a, nextValue, count - 1);
}
int main() {
  double a = 0.1, x1 = 0.1;
  printf("aを入力してください\n");
  scanf("%lf", &a);
  printf("x1を入力してください\n");
  scanf("%lf", &x1);
  double res = limitfunc(a, x1, 100000);
  printf("%lf", res);
  return 0;
}