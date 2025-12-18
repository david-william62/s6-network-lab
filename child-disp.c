#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void main() {
 int n,i;
 n = fork();

 if(n>0) {
  printf("-----PARENT PROCESS-----");
  printf("\nparent\n");
 } else {
  printf("-----CHILD PROCESS-----");
  printf("\nchild\n");
 }
}
