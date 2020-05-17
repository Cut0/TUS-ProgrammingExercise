//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
typedef struct Person {
  double height;
  double weight;
  char blood[2];
} person;
void setData(int N, person *people) {
  int i;
  for (i = 0; i < N; i++) {
    printf("%d人目のデータを入力\n", i + 1);
    printf("身長(cm):");
    scanf("%lf", &people[i].height);
    printf("体重(kg):");
    scanf("%lf", &people[i].weight);
    printf("血液型:");
    scanf("%s", people[i].blood);
  }
}
void printData(int N, person *people) {
  int i;
  for (i = 0; i < N; i++) {
    printf("%d人目のデータを出力\n", i + 1);
    printf("身長(cm):%f\n", people[i].height);
    printf("体重(kg):%f\n", people[i].weight);
    printf("血液型:%s\n", people[i].blood);
  }
}
int main() {
  int N, i;
  printf("人数を入力:");
  scanf("%d", &N);
  person people[N];
  setData(N, people);
  printData(N, people);
  return 0;
}