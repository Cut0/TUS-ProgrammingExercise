//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

bool is_uruu(int year) {
  return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

int main() {
  int year, date;
  int data[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  printf("西暦を入力してください。");
  scanf("%d", &year);
  printf("月を入力してください");
  scanf("%d", &date);
  if (date == 2 && is_uruu(year)) {
    printf("29日です");
  } else {
    printf("%d日です", data[date - 1]);
  };

  return 0;
}