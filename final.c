#include <stdio.h>      // Include standard input/output library
#include <stdlib.h>     // Include standard library (for malloc, free, etc.)
#include <string.h>     // Include string manipulation library
#include <ctype.h>      // Include character type checking library
#define MAX_TOKENS 100  // Define maximum number of tokens
#define MAX_ID_LEN 50   // Define maximum length of an identifier

typedef struct {        // Structure to store symbol table entries
    int index;         // Index of the symbol in the table
    char name[MAX_ID_LEN]; // Name of the identifier
    char type[10];     // Data type of the identifier
    int size;          // Size of the identifier (e.g., in bytes)
} Symbol;

Symbol symbolTable[MAX_TOKENS]; // Declare the symbol table as an array of Symbol structures
int symbolCount = 0;            // Initialize the count of symbols in the table

int lookupSymbol(char *name) {  // Function to check if an identifier already exists in the symbol table
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0)
            return symbolTable[i].index;
    }
    return -1;
}

int insertSymbol(char *name, char *type, int size) {  // Function to insert a new identifier into the symbol table
    int index = lookupSymbol(name);
    if (index == -1) {
        symbolTable[symbolCount].index = symbolCount + 1;
        strcpy(symbolTable[symbolCount].name, name);
        strcpy(symbolTable[symbolCount].type, type);
        symbolTable[symbolCount].size = size;
        symbolCount++;
        return symbolTable[symbolCount - 1].index;
    }
    return index;
}

// Modified keyword check: Removed "include" and "main" from the list.
int isKeyword(const char *str) {
    char *keywords[] = {
        "if", "else", "while", "for", "do", "int", "float", "char", "return", "void"
    };
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

void getNextToken(FILE *fp) {
    char ch, buffer[MAX_ID_LEN];
    int line = 1, col = 0, tokenIndex = 1;
    
    while ((ch = fgetc(fp)) != EOF) {
        col++;
        
        // Handle preprocessor directives (e.g., #include <...>)
        if(ch == '#') {
            int i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            printf("%d. < Preprocessor , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
            if(ch == '\n'){
                line++;
                col = 0;
            }
            continue;
        }
        
        if (isspace(ch)) {
            if (ch == '\n') {
                line++;
                col = 0;
            }
            continue;
        }
        
        // Handle character literals (e.g., 'x')
        if(ch == '\'') {
            int i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != '\'' && ch != EOF) {
                buffer[i++] = ch;
            }
            buffer[i++] = '\'';  // closing quote
            buffer[i] = '\0';
            printf("%d. < Character , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
            continue;
        }
        
        // Handle identifiers and keywords
        if (isalpha(ch) || ch == '_') {
            int i = 0;
            buffer[i++] = ch;
            while (isalnum(ch = fgetc(fp)) || ch == '_') {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, fp);
            if (isKeyword(buffer)) {
                printf("%d. < Keyword , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
            } else if (strcmp(buffer, "printf") == 0) {
                // Tokenize printf as an identifier but DO NOT add it to the symbol table.
                printf("%d. < Identifier , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
            } else {
                int index = insertSymbol(buffer, "var", 4);
                printf("%d. < Identifier , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
            }
        } else if (isdigit(ch)) {
            int i = 0;
            buffer[i++] = ch;
            while (isdigit(ch = fgetc(fp))) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, fp);
            printf("%d. < Number , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
        } else if (ch == '=' || ch == '>' || ch == '<' || ch == '!') {
            buffer[0] = ch;
            ch = fgetc(fp);
            if (ch == '=') {
                buffer[1] = ch;
                buffer[2] = '\0';
            } else {
                buffer[1] = '\0';
                ungetc(ch, fp);
            }
            printf("%d. < Symbol , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (ch == '/' && (ch = fgetc(fp)) == '/') { // Single-line comment
                while ((ch = fgetc(fp)) != '\n' && ch != EOF);
                line++;
                col = 0;
                continue;
            } else if (ch == '/' && ch == '*') { // Multi-line comment (minimal change; original logic preserved)
                while ((ch = fgetc(fp)) != EOF) {
                    if (ch == '*' && (ch = fgetc(fp)) == '/') break;
                }
                continue;
            } else {
                buffer[0] = ch;
                buffer[1] = '\0';
                printf("%d. < Operator , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
            }
        } else if (ch == '&' || ch == '|') {
            buffer[0] = ch;
            ch = fgetc(fp);
            if (ch == buffer[0]) {
                buffer[1] = ch;
                buffer[2] = '\0';
            } else {
                buffer[1] = '\0';
                ungetc(ch, fp);
            }
            printf("%d. < Logical Operator , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
        } else if (ch == '"') {
            int i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fp)) != '"' && ch != EOF) {
                buffer[i++] = ch;
            }
            buffer[i++] = '"';
            buffer[i] = '\0';
            printf("%d. < String , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
        } else {
            // For any other single character (e.g., punctuation), treat it as a symbol.
            buffer[0] = ch;
            buffer[1] = '\0';
            printf("%d. < Symbol , %d , %d, %s>\n", tokenIndex++, line, col, buffer);
        }
    }
}

void printSymbolTable() {
    printf("\nSymbol Table:\n");
    printf("Index   Name    Type    Size\n");
    printf("-----   ----    ----    ----\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%d       %s      %s      %d\n",
               symbolTable[i].index,
               symbolTable[i].name,
               symbolTable[i].type,
               symbolTable[i].size);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }
    getNextToken(fp);
    fclose(fp);
    printSymbolTable();
    return 0;
}
