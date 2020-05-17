//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void test(int *nums) {
  int i;
  for (i = 0; i < 3; i++) {
    printf("%d", nums[i]);
  }
}
int main() {
  int nums[3] = {1, 2, 3};
  test(nums);
  return 0;
}