#include <stdio.h>
#include <stdlib.h>

/* Test program with multiple spaces and tabs */
int main() {
 int x = 10;
 if (x > 5) {
 printf("Hello World!\n");
 }
 
 while (x > 0) {
 x--;
 }
 
 #define MAX 100
 #ifdef DEBUG
 printf("Debug mode\n");
 #endif
 
 return 0;
}