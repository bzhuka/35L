#include <stdio.h>

int main(int argc, char** argv) {
  if (argc !=3 && argc !=1) {
    fprintf( stderr, "wrong number of operands");
    return 1;
  }
  if (argc == 3 && dup(argv[1])==1) {
    fprintf( stderr, "duplicates in from");
    return 1;
  }
  if (argc == 3 && sameln(argv[1], argv[2]) != 0) {
    fprintf( stderr, "two operands not same length");
    return 1;
  }
  int c = getchar();
  
  while (c != EOF) {
    if (argc == 3) {
      putchar(trans(c, argv[1], argv[2]));
    } else {
      putchar(c);
    }
    c = getchar();
  }
  return 0;
}

int trans(int c, char* fro, char* to) {
  int i = 0;
  while (fro[i] != '\0') {
    if(fro[i] == c) {
      return to[i];
    }
    i++;
  }
  return c;
}

int dup(char* fro) {
  int i = 0;
  int j = 1;
  while(fro[i] != '\0') {
    while(fro[j] != '\0') {
      if (fro[i] == fro[j]) {
	return 1;
      } else {
	j++;
      }
    }
    i++;
    j = i+1;
  }
  return 0;
}

int sameln(char* a1, char* a2) {
  int i = 0;
  while(a1[i] !='\0') {
    i++;
  }
  int j = 0;
  while (a2[j] != '\0') {
    j++;
  }
  return i-j;
}
