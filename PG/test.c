//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define W 5
int powN(int a, int b) {
  int i, result = 1;
  for (i = 0; i < b; i++) {
    result = result * a;
  }
  return result;
}
int main() {
  int num = powN(0, 2);
  printf("%d", num);
  return 0;
}
