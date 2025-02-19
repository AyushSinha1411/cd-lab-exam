// Program 1: Replace multiple spaces/tabs with single space
#include <stdio.h>
#include <stdlib.h>

void normalizeSpaces(char* inFile, char* outFile) {
    FILE *fin, *fout;
    int ch, prevChar = ' ';
    
    fin = fopen(inFile, "r");
    if (fin == NULL) {
        printf("Cannot open input file\n");
        exit(1);
    }
    
    fout = fopen(outFile, "w");
    if (fout == NULL) {
        printf("Cannot create output file\n");
        fclose(fin);
        exit(1);
    }
    
    while ((ch = getc(fin)) != EOF) {
        if (ch == ' ' || ch == '\t') {
            if (prevChar != ' ')
                putc(' ', fout);
            prevChar = ' ';
        } else {
            putc(ch, fout);
            prevChar = ch;
        }
    }
    
    fclose(fin);
    fclose(fout);
}
int main() {
    // Test all three functions
    normalizeSpaces("input.c", "output1.c");
    return 0;
}