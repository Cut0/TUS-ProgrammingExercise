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
int hashVal(int a) {
  int i = 0, h = 0;
  for (i = W; i > 0; i--) {
    h += a / pow(10, i);
    printf("%d:", (int)pow(10, i));
    printf("%d\n", (int)(a / pow(10, i)));
    a = a % (int)pow(10, i);
  }
  h += a;
  return h;
}
typedef struct Cell {
  int key;
  int dist;
  int parent;
  int next;
} Cell;
int main() {
  printf("%d", hashVal(2115));
  return 0;
}