#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int totcomp = 0;
int frobcmp(const void* a, const void* b) {
  totcomp+=1;
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
  struct stat meta;
  fstat(0, &meta);
  int filesize = meta.st_size;
  
  int buff = 1024;
  int empty = 0;
  char* arr;
  int i = 0;
  
  if (S_ISREG(meta.st_mode)) {
    arr = (char*) malloc(sizeof(char) * filesize);
    int filesize1 = filesize;
    
    if (arr == NULL) {
      fprintf(stderr, "Freeing memory failed.");
      exit(1);
    }

    int previous = ' ';
    char c;
    int counter = 0;
    //fprintf(stderr, "Size: %d", filesize);
    while (counter < filesize) {
      //fprintf(stderr, "hi %d", counter);
      read(0, &c, 1);
      if (!(c == ' ' && (char)previous == ' ')) {
	empty+=1;
	if (counter == filesize1) {
	  fprintf(stderr, "failed");
	  arr = (char*) realloc(arr, sizeof(char) * filesize);
	  if (arr == NULL) {
	    fprintf(stderr, "Reallocating memory failed.");
	    exit(1);
	  }
	  filesize1 = filesize;
	}
	arr[i] = c;
	i++;
      }
      counter++;
      fstat(0, &meta);
      filesize = meta.st_size;
      previous = c;
    }
  } else {
    arr = (char*) malloc(sizeof(char) * buff);
    
    if (arr == NULL) {
      fprintf(stderr, "Freeing memory failed.");
      exit(1);
    }

    int previous = ' ';
        char c;
    while (read(0, &c, 1) > 0) {
      if (!(c == ' ' && (char)previous == ' ')) {
	empty+=1;
	if (i == buff) {
	  arr = (char*) realloc(arr, sizeof(char) * buff * 2);
	  buff = buff * 2;
	  if (arr == NULL) {
	    fprintf(stderr, "Reallocating memory failed.");
	    exit(1);
	  }
	}
	arr[i] = c;
	i++;
      }
      previous = c;
    }  
  }
  //fprintf( stderr, "made it %d", i);
  if (empty == 0) {
    fprintf(stderr, "Comparisons: 0");
    free(arr);
    return 0;
  }
    
  if (arr[i-1] != ' ') {
    if (i == buff || i == filesize) {
      buff = filesize;
      arr = (char*) realloc(arr, sizeof(char) * buff + sizeof(char));
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
      write(1, &(*printplz), 1);
      printplz++;
    }
  }

  free(arr);
  free(tmparr);
  free(len);

  fprintf(stderr, "Comparisons: %d\n", totcomp);
  exit(0);
}
