//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
  int i, k;
  int count = 1000000;             //乱数の個数,10000
  int add_count = 100;             //加算する回数
  int shikou = count / add_count;  //和のデータ数
  int data[add_count - 1];
  for (i = 0; i < 99; i++) {
    data[i] = 0;
  }

  FILE *fp;
  fp = fopen("data.txt", "r");
  if (fp == NULL) {
    printf("ファイル開けませんよ!\n");
    return 1;
  }

  for (i = 0; i < shikou; i++) {
    int num = 0;
    for (k = 0; k < add_count; k++) {
      int tmp = 0;
      fscanf(fp, "%d", &tmp);
      num += tmp;
    }
    int pos = num / add_count;
    data[pos] += 1;
  }
  bool show = false;
  for (i = 0; i < 99; i++) {
    if (data[i] != 0) {
      show = true;
    }
    if (data[i] == 0) {
      show = false;
    }
    if (show) {
      printf("%d~%d:%d個\n", i * add_count, (i + 1) * add_count - 1, data[i]);
    }
  }

  return 0;
}