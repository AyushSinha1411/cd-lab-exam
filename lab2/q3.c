#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NUM_KEYWORDS 32

const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
    "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
    "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while"
};

int isKeyword(const char *word) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void processFile(FILE *input, FILE *output) {
    char word[256];
    int ch, index = 0;

    while ((ch = fgetc(input)) != EOF) {
        if (isalnum(ch) || ch == '_') {
            word[index++] = ch;
        } else {
            if (index > 0) {
                word[index] = '\0';
                if (isKeyword(word)) {
                    for (int i = 0; word[i]; i++) {
                        word[i] = toupper(word[i]);
                    }
                }
                fputs(word, output);
                index = 0;
            }
            fputc(ch, output);
        }
    }
}

int main() {
    FILE *input = fopen("input.c", "r");
    FILE *output = fopen("output3.c", "w");

    if (!input || !output) {
        printf("Error: Unable to open files.\n");
        return 1;
    }

    processFile(input, output);

    fclose(input);
    fclose(output);
    printf("Keywords capitalized. Check output3.c\n");
    return 0;
}
