#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function to identify arithmetic operators
void identifyArithmeticOperators(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
        printf("Arithmetic Operator: %c\n", c);
    }
}

// Function to identify relational operators
void identifyRelationalOperators(char c, FILE *fp) {
    if (c == '=' || c == '<' || c == '>' || c == '!') {
        char nextChar = fgetc(fp);
        if (nextChar == '=') {
            printf("Relational Operator: %c%c\n", c, nextChar); // For operators like <=, >=, !=, ==
        } else {
            ungetc(nextChar, fp); // Push back the character if it's not part of a two-char operator
            printf("Relational Operator: %c\n", c); // For operators like <, >, =
        }
    }
}

// Function to identify logical operators
void identifyLogicalOperators(char c, FILE *fp) {
    if (c == '&' || c == '|') {
        char nextChar = fgetc(fp);
        if (nextChar == c) {
            printf("Logical Operator: %c%c\n", c, nextChar); // For && or ||
        } else {
            ungetc(nextChar, fp); // Push back the character if it's not part of a two-char operator
        }
    } else if (c == '!') {
        printf("Logical Operator: %c\n", c); // For !
    }
}

// Function to identify special symbols
void identifySpecialSymbols(char c) {
    if (c == '(') printf("Special Symbol: ( \n");
    if (c == ')') printf("Special Symbol: ) \n");
    if (c == '{') printf("Special Symbol: { \n");
    if (c == '}') printf("Special Symbol: } \n");
    if (c == '[') printf("Special Symbol: [ \n");
    if (c == ']') printf("Special Symbol: ] \n");
    if (c == ';') printf("Special Symbol: ; \n");
    if (c == ',') printf("Special Symbol: , \n");
    if (c == '#') printf("Special Symbol: # \n"); // Handle the '#' symbol as part of preprocessor directives
}

// Function to identify preprocessor directives (like #include)
void identifyPreprocessorDirective(char *lexeme) {
    if (lexeme[0] == '#') {
        printf("Preprocessor Directive: %s\n", lexeme);
    }
}

// Function to identify keywords
void identifyKeywords(char *lexeme) {
    char *keywords[] = {"int", "char", "if", "else", "return", "for", "while", "void"};
    for (int i = 0; i < 8; i++) {
        if (strcmp(lexeme, keywords[i]) == 0) {
            printf("Keyword: %s\n", lexeme);
            return;
        }
    }
}

// Function to identify numerical constants
void identifyNumericalConstants(char *lexeme) {
    int isNumber = 1;
    for (int i = 0; lexeme[i] != '\0'; i++) {
        if (!isdigit(lexeme[i])) {
            isNumber = 0;
            break;
        }
    }
    if (isNumber) {
        printf("Numerical Constant: %s\n", lexeme);
    }
}

// Function to identify string literals
void identifyStringLiterals(char *lexeme) {
    if (lexeme[0] == '"' && lexeme[strlen(lexeme)-1] == '"') {
        printf("String Literal: %s\n", lexeme);
    }
}

// Function to identify character constants (e.g., 'A')
void identifyCharacterConstants(char *lexeme) {
    if (lexeme[0] == '\'' && lexeme[strlen(lexeme)-1] == '\'') {
        printf("Character Constant: %s\n", lexeme);
    }
}

// Function to identify identifiers
void identifyIdentifiers(char *lexeme) {
    if (isalpha(lexeme[0]) || lexeme[0] == '_') {
        int isValidIdentifier = 1;
        for (int i = 1; lexeme[i] != '\0'; i++) {
            if (!isalnum(lexeme[i]) && lexeme[i] != '_') {
                isValidIdentifier = 0;
                break;
            }
        }
        if (isValidIdentifier) {
            printf("Identifier: %s\n", lexeme);
        }
    }
}

// Main function to process the input file and identify all tokens
int main() {
    FILE *fp = fopen("sample.c", "r"); // Open the source file
    if (fp == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    char c;
    char lexeme[100];
    int lexemeIndex = 0;

    // Process each character of the input file
    while ((c = fgetc(fp)) != EOF) {
        if (isspace(c)) {
            continue;  // Skip whitespaces
        }

        // Handle preprocessor directive (#include, etc.)
        if (c == '#') {
            lexeme[lexemeIndex++] = c;
            while ((c = fgetc(fp)) != '\n' && c != EOF) {
                lexeme[lexemeIndex++] = c;
            }
            lexeme[lexemeIndex] = '\0';
            identifyPreprocessorDirective(lexeme);
            lexemeIndex = 0;
        }

        // Handle identifiers and keywords
        else if (isalpha(c) || c == '_') {
            lexeme[lexemeIndex++] = c;
            while (isalnum(c = fgetc(fp)) || c == '_') {
                lexeme[lexemeIndex++] = c;
            }
            lexeme[lexemeIndex] = '\0';
            ungetc(c, fp);  // Push back the character after the identifier
            identifyKeywords(lexeme);
            identifyIdentifiers(lexeme);
            lexemeIndex = 0;
        }

        // Handle numerical constants
        else if (isdigit(c)) {
            lexeme[lexemeIndex++] = c;
            while (isdigit(c = fgetc(fp))) {
                lexeme[lexemeIndex++] = c;
            }
            lexeme[lexemeIndex] = '\0';
            ungetc(c, fp);
            identifyNumericalConstants(lexeme);
            lexemeIndex = 0;
        }

        // Handle string literals
        else if (c == '"') {
            lexeme[lexemeIndex++] = c;
            while ((c = fgetc(fp)) != '"' && c != EOF) {
                lexeme[lexemeIndex++] = c;
            }
            lexeme[lexemeIndex++] = c;
            lexeme[lexemeIndex] = '\0';
            identifyStringLiterals(lexeme);
            lexemeIndex = 0;
        }

        // Handle character literals (e.g., 'A')
        else if (c == '\'') {
            lexeme[lexemeIndex++] = c;
            while ((c = fgetc(fp)) != '\'' && c != EOF) {
                lexeme[lexemeIndex++] = c;
            }
            lexeme[lexemeIndex++] = c;
            lexeme[lexemeIndex] = '\0';
            identifyCharacterConstants(lexeme);
            lexemeIndex = 0;
        }

        // Handle special symbols and operators
        else {
            identifySpecialSymbols(c);
            identifyArithmeticOperators(c);
            identifyRelationalOperators(c, fp);
            identifyLogicalOperators(c, fp);
        }
    }

    fclose(fp);
    return 0;
}

