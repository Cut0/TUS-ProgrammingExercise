//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
  srand((unsigned int)time(NULL));
  int validNum = rand() % 100;
  while (true) {
    int num = 0;
    printf("数値を入力してください");
    scanf("%d", &num);
    if (num - validNum >= 10)
      printf("大きすぎます\n");
    else if (num - validNum > 0)
      printf("大きいです\n");
    else if (num - validNum < -10)
      printf("小さすぎます\n");
    else if (num - validNum < 0)
      printf("小さいです\n");
    else {
      break;
    }
  }
  printf("正解です");
  return 0;
}