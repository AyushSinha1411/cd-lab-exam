#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const char* preprocessorKeywords[] = {
    "include", "define", "undef", "ifdef", "ifndef", "if", "else", "elif",
    "endif", "line", "error", "pragma", "warning"
};

int isPreprocessorKeyword(const char* word) {
    for (int i = 0; i < sizeof(preprocessorKeywords) / sizeof(preprocessorKeywords[0]); i++) {
        if (strcmp(word, preprocessorKeywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    FILE *fin, *fout;
    int ch;
    char word[256];
    
    fin = fopen("input2.c", "r");
    if (fin == NULL) {
        printf("Cannot open input file\n");
        exit(1);
    }
    
    fout = fopen("output2.c", "w");
    if (fout == NULL) {
        printf("Cannot create output file\n");
        fclose(fin);
        exit(1);
    }
    
    while ((ch = getc(fin)) != EOF) {
        if (ch == '#') {
            int pos = 0;
            ch = getc(fin);
            
            while (ch != EOF && isspace(ch)) {
                ch = getc(fin);
            }
            
            while (ch != EOF && isalpha(ch) && pos < 255) {
                word[pos++] = ch;
                ch = getc(fin);
            }
            word[pos] = '\0';
            
            if (isPreprocessorKeyword(word)) {
              while (ch != EOF && ch != '\n') {
                    ch = getc(fin);
                }
                putc('\n', fout);
            } else {
                 putc('#', fout);
                fprintf(fout, "%s", word);
                if (ch != EOF) {
                    putc(ch, fout);
                }
            }
        } else {
            putc(ch, fout);
        }
    }
    
    fclose(fin);
    fclose(fout);
    return 0;
}
