#include <stdio.h>
#include <stdlib.h>

int frobcmp(const void* a, const void* b) {
  const char *f = *(const char **)a;
  const char *s = *(const char **)b;

  while(1) {
    int fc = (int)(*f);
    int sc = (int)(*s);

      if (fc == 32 && sc == 32) {
	return 0;
      } else if (fc == 32) {
	return -1;
      } else if (sc == 32) {
	return 1;
      }
      fc = fc^'*';
      sc = sc^'*';

      if (fc - sc != 0) {
	return fc-sc;
      }
      f++;
      s++;
  }
  return 0;
}

int main() {
  int buff = 10;
  char* arr = (char*) malloc(sizeof(char) * buff);
  int i = 0;

  if (arr == NULL) {
    fprintf(stderr, "Freeing memory failed.");
    exit(1);
  }

  int previous = ' ';
  int j;
  int empty = 0;
  while (1) {
    j = getchar();
    if (j == EOF) {
      break;
    }
    if (!((char) j == ' ' && (char)previous == ' ')) {
      empty+=1;
      if (i == buff) {
	arr = (char*) realloc(arr, sizeof(char) * buff * 2);
	buff = buff * 2;
	if (arr == NULL) {
	  fprintf(stderr, "Reallocating memory failed.");
	  exit(1);
	}
      }
      arr[i] = (char) j;
      i++;
    }
    previous = j;
  }

  if (empty == 0) {
    free(arr);
    return 0;
  }
  if (arr[i-1] != ' ') {
    if (i == buff) {
      buff = buff * 2;
      arr = (char*) realloc(arr, sizeof(char) * buff);
      if (arr == NULL) {
	fprintf(stderr, "Reallocating memory failed.");
	exit(1);
      }
    }
    arr[i] = ' ';
    i++;
  }
  
  int spaces = 0;
  for (int k = 0; k < i; k++) {
    if (arr[k] == ' ') {
      spaces++;
    }
  }

  char **tmparr = (char**) malloc(sizeof(char*) * spaces);
  int *len = (int*) malloc(sizeof(int) * spaces);
  if (tmparr == NULL || len == NULL) {
    fprintf(stderr, "Allocating memory failed.");
    exit(1);
  }

  int index = 0;
  char *ptr = arr;
  int tmpsize = 0;
  tmparr[index] = ptr;
  
  for (int k = 0; k < i; k++) {
    if (arr[k] == ' ') {
      index++;
    }
    if (k != 0 && arr[k-1] == ' ') {
      tmparr[index] = ptr;
    }
    ptr++;
  }
  
  /*
  for (int k = 0; k < spaces; k++) {
    char *printplz = tmparr[k];
    for (int l = 0; l < len[k]; l++) {
      printf("%c", *printplz);
      printplz++;
    }
  }
  */
  qsort(tmparr, index, sizeof(char*), frobcmp);

  for (int k = 0; k < spaces; k++) {
    char *isspace = tmparr[k];
    while(*isspace != ' ') {
      isspace++;
      tmpsize++;
    }
    tmpsize++;
    len[k] = tmpsize;
    tmpsize = 0;
  }

  //  printf("%d", spaces);
    for (int k = 0; k < spaces; k++) {
      char *printplz = tmparr[k];
    for (int l = 0; l < len[k]; l++) {
      //printf("%c", *printplz);
      //printplz++;
      putchar(*printplz++);
    }
  }

  free(arr);
  free(tmparr);
  free(len);

  exit(0);
}
