//学籍番号: 4619023 氏名:　加藤　零
#define NUM 10
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int GetRandom(int min, int max) {
  return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

int main() {
  int count = 1000000, i;
  FILE *outputfile;  // 出力ストリーム

  outputfile = fopen("data.txt", "w");  // ファイルを書き込み用にオープン(開く)
  if (outputfile == NULL) {   // オープンに失敗した場合
    printf("cannot open\n");  // エラーメッセージを出して
    exit(1);                  // 異常終了
  }
  for (i = 0; i < count; i++) {
    fprintf(outputfile, "%d\n", GetRandom(0, 99));
  }
  fclose(outputfile);  // ファイルをクローズ(閉じる)
  return 0;
}