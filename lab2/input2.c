#include <stdio.h>
#define MAX 100
int main() {
    int #90 = 90;  // This # will be preserved
    printf("Line #1\n");  // This # will be preserved
    #ifdef DEBUG
    printf("Debug mode\n");
    #endif
    return 0;
}
