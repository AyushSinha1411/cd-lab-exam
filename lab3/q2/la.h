// Lexical Analyser, getNextToken()
// Inside a literal, preprocessors, comments discard, start from main()
// Write line number and column number

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct token {
    char lexeme[64];
    int row, col;
    char type[30];
};

static int row = 1, col = 1;
char buf[1024];
char specialsymbols[] = {'?', ';', ':', ',', '(', ')', '{', '}', '.'};
char *Keywords[] = {"for", "if", "else", "while", "do", "break", "continue", "return", "int", "double", "float", "char", 
							"long", "short", "sizeof", "typedef", "switch", "case", "struct", "const", "void", "exit"};
char arithmeticsymbols[] = {'*','+','-','/', '%'};

int isKeyword(char *str) 
{
    for (int i = 0; i < sizeof(Keywords) / sizeof(char *); i++) {
        if (strcmp(str, Keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int charBelongsTo(int c, char *arr) {
    int len;
    if (arr == specialsymbols)
        len = sizeof(specialsymbols) / sizeof(char);
    else if (arr == arithmeticsymbols)
        len = sizeof(arithmeticsymbols) / sizeof(char);
    for (int i = 0; i < len; i++) {
        if (c == arr[i])
            return 1;
    }
    return 0;
}

void fillToken(struct token *tkn, char c, int row, int col, char *type) {
    tkn->row = row;
    tkn->col = col;
    strcpy(tkn->type, type);
    tkn->lexeme[0] = c;
    tkn->lexeme[1] = '\0';
}

void newLine() {
    ++row;
    col = 1;
}

struct token getNextToken(FILE *fin) {
    int c, d;
    struct token tkn = {.row = -1};
    int gotToken = 0;
    while (!gotToken && (c = getc(fin)) != EOF) {

        // SKIP COMMENTS
        if (c == '/') {

        	// Skip over single-line comments (//)
            d = getc(fin);
            if (d == '/') {
                while ((c = getc(fin)) != EOF && c != '\n') {
                    ++col;
                }
                if (c == '\n') {
                    newLine();
                }
                continue;
            }

            // Skip over multi-line comments (/* */)
            else if (d == '*') {
                do {
                    if (c == '\n') newLine();
                    while ((c = getc(fin)) != EOF && c != '*') {
                        ++col;
                        if (c == '\n') newLine();
                    }
                    if (c == '*') {
                        d = getc(fin);
                        ++col;
                    }
                } while (c != EOF && d != '/');  // Loop until we find closing */
                continue;
            } 
            else {
                fseek(fin, -1, SEEK_CUR);  // Not a comment, put back the character
            }
        }

        // Handle preprocessor directives (lines starting with '#')
        if (c == '#') {
            tkn.row = row;
            tkn.col = col++;
            tkn.lexeme[0] = '#';
            int k = 1;
            while ((c = getc(fin)) != '\n' && !isspace(c)) {
                tkn.lexeme[k++] = c;  // Collect the rest of the directive
                col++;
            }
            tkn.lexeme[k] = '\0';  // Null-terminate the lexeme

            // Check if it is a valid #include directive
            if (strcmp(tkn.lexeme, "#include") == 0) {
                // Look for opening angle bracket '<' or double quote '"'
                c = getc(fin);
                ++col;
                if (c == '<') {
                    // Read until closing angle bracket '>'
                    while ((c = getc(fin)) != '\n' && c != '>') {
                        ++col;
                    }
                    if (c == '>') {
                        // Successfully processed #include <...> directive, discard it
                        while ((c = getc(fin)) != '\n' && c != '\n') {
                            ++col;  // Skip to the next line
                        }
                        if (c == '\n') {
                            newLine();
                        }
                        continue;  // Skip processing this directive, no token needed
                    }
                    else {
                        // If we didn't find '>', treat as invalid and tokenize
                        tkn.row = row;
                        tkn.col = col;
                        strcpy(tkn.type, "InvalidDirective");
                        strcpy(tkn.lexeme, "#include<");
                        // Continue reading the rest of the line as part of the invalid directive
                        while ((c = getc(fin)) != '\n' && c != '\n') {
                            strncat(tkn.lexeme, (char*)&c, 1);
                            ++col;
                        }
                        if (c == '\n') {
                            newLine();
                        }
                        gotToken = 1;
                    }
                }
                else if (c == '"') {
                    // Read until closing quote '"'
                    while ((c = getc(fin)) != '\n' && c != '"') {
                        ++col;
                    }
                    if (c == '"') {
                        // Successfully processed #include "..." directive, discard it
                        while ((c = getc(fin)) != '\n' && c != '\n') {
                            ++col;  // Skip to the next line
                        }
                        if (c == '\n') {
                            newLine();
                        }
                        continue;  // Skip processing this directive, no token needed
                    }
                    else {
                        // If we didn't find '"', treat as invalid and tokenize
                        tkn.row = row;
                        tkn.col = col;
                        strcpy(tkn.type, "InvalidDirective");
                        strcat(tkn.lexeme, "\"");
                        while ((c = getc(fin)) != '\n' && c != '\n') {
                            strncat(tkn.lexeme, (char*)&c, 1);
                            ++col;
                        }
                        if (c == '\n') {
                            newLine();
                        }
                        gotToken = 1;
                    }
                }
                else {
                    // If the next character isn't '<' or '"', treat as invalid and tokenize
                    tkn.row = row;
                    tkn.col = col;
                    strcpy(tkn.type, "InvalidDirective");
                    //strcat(tkn.lexeme, "<invalid>");
                    while ((c = getc(fin)) != '\n' && c != '\n') {
                        strncat(tkn.lexeme, (char*)&c, 1);
                        ++col;
                    }
                    if (c == '\n') {
                        newLine();
                    }
                    gotToken = 1;
                }
            }
        }

        // Process special symbols
        if (charBelongsTo(c, specialsymbols)) {
            switch(c) {
                case ';':
                    fillToken(&tkn, c, row, col, ";");
                    break;
                case ':':
                    fillToken(&tkn, c, row, col, ":");
                    break;
                case '?':
                    fillToken(&tkn, c, row, col, "?");
                    break;
                case ',':
                    fillToken(&tkn, c, row, col, ",");
                    break;
                case '(':
                    fillToken(&tkn, c, row, col, "(");
                    break;
                case ')':
                    fillToken(&tkn, c, row, col, ")");
                    break;
                case '{':
                    fillToken(&tkn, c, row, col, "{");
                    break;
                case '}':
                    fillToken(&tkn, c, row, col, "}");
                    break;
                case '.':
                    fillToken(&tkn, c, row, col, ".");
                    break;
                default: break;
            }
            gotToken = 1;
            ++col;
        }

        // Process arithmetic operators
        else if (charBelongsTo(c, arithmeticsymbols)) {
            switch(c) {
                case '+':
                    d = getc(fin);
                    if (d == '+') {
                        fillToken(&tkn, c, row, col, "++");
                        strcpy(tkn.lexeme, "++");
                        gotToken = 1;
                        col += 2;
                    } else if (d == '=') {
                        fillToken(&tkn, c, row, col, "+=");
                        strcpy(tkn.lexeme, "+=");
                        gotToken = 1;
                        col += 2;
                    } else {
                        fillToken(&tkn, c, row, col, "+");
                        gotToken = 1;
                        ++col;
                        fseek(fin, -1, SEEK_CUR);
                    }
                    break;
                case '-':
                    d = getc(fin);
                    if (d == '-') {
                        fillToken(&tkn, c, row, col, "--");
                        strcpy(tkn.lexeme, "--");
                        gotToken = 1;
                        col += 2;
                    } else if (d == '=') {
                        fillToken(&tkn, c, row, col, "-=");
                        strcpy(tkn.lexeme, "-=");
                        gotToken = 1;
                        col += 2;
                    } else {
                        fillToken(&tkn, c, row, col, "-");
                        gotToken = 1;
                        ++col;
                        fseek(fin, -1, SEEK_CUR);
                    }
                    break;
                case '*':
                    d = getc(fin);
                    if (d != '=') {
                        fillToken(&tkn, c, row, col, "*");
                        gotToken = 1;
                        ++col;
                        fseek(fin, -1, SEEK_CUR);
                    } else {
                        fillToken(&tkn, c, row, col, "*=");
                        strcpy(tkn.lexeme, "*=");
                        gotToken = 1;
                        col += 2;
                    }
                    break;
                case '/':
                    d = getc(fin);
                    if (d == '=') {
                        fillToken(&tkn, c, row, col, "/=");
                        strcpy(tkn.lexeme, "/=");
                        gotToken = 1;
                        col += 2;
                    } else {
                        fillToken(&tkn, c, row, col, "/");
                        gotToken = 1;
                        ++col;
                        fseek(fin, -1, SEEK_CUR);
                    }
                    break;
                case '%':
                    d = getc(fin);
                    if (d != '=') {
                        fillToken(&tkn, c, row, col, "%");
                        gotToken = 1;
                        ++col;
                        fseek(fin, -1, SEEK_CUR);
                    } else {
                        fillToken(&tkn, c, row, col, "%=");
                        strcpy(tkn.lexeme, "%=");
                        gotToken = 1;
                        col += 2;
                    }
                    break;
                default : break;
            }
            gotToken = 1;
            ++col;
        }

        // Process relational operators
        else if (c == '=' || c == '<' || c == '>' || c == '!') {
            switch(c) {
                case '=':
                    d = getc(fin);
                    if (d == '=') {
                        fillToken(&tkn, c, row, col, "==");
                        strcpy(tkn.lexeme, "==");
                        gotToken = 1;
                        col += 2;
                    } else {
                        fillToken(&tkn, c, row, col, "=");
                        gotToken = 1;
                        ++col;
                        fseek(fin, -1, SEEK_CUR);
                    }
                    break;
                case '<':
                    fillToken(&tkn, c, row, col, "<");
                    ++col;
                    d = getc(fin);
                    if (d == '=') {
                        ++col;
                        strcat(tkn.lexeme, "=");
                    } else {
                        fseek(fin, -1, SEEK_CUR);
                    }
                    gotToken = 1;
                    break;
                case '>':
                    fillToken(&tkn, c, row, col, ">");
                    ++col;
                    d = getc(fin);
                    if (d == '=') {
                        ++col;
                        strcat(tkn.lexeme, "=");
                    } else {
                        fseek(fin, -1, SEEK_CUR);
                    }
                    gotToken = 1;
                    break;
                case '!':
                    fillToken(&tkn, c, row, col, "!");
                    ++col;
                    d = getc(fin);
                    if (d == '=') {
                        ++col;
                        strcat(tkn.lexeme, "=");
                        strcpy(tkn.type, "!=");
                    } else {
                        fseek(fin, -1, SEEK_CUR);
                    }
                    gotToken = 1;
                    break;
                default: break;
            }
        }

        // Process numbers
        else if (isdigit(c)) {
            tkn.row = row;
            tkn.col = col++;
            tkn.lexeme[0] = c;
            int k = 1;
            while ((c = getc(fin)) != EOF && isdigit(c)) {
                tkn.lexeme[k++] = c;
                col++;
            }
            tkn.lexeme[k] = '\0';
            strcpy(tkn.type, "Number");
            gotToken = 1;
            fseek(fin, -1, SEEK_CUR);
        }

        // Discard whitespaces
        else if (isspace(c)) {
            ++col;
        }

        // Process keywords and identififers
        else if (isalpha(c) || c == '_') {
            tkn.row = row;
            tkn.col = col++;
            tkn.lexeme[0] = c;
            int k = 1;
            while ((c = getc(fin)) != EOF && isalnum(c)) {
                tkn.lexeme[k++] = c;
                ++col;
            }
            tkn.lexeme[k] = '\0';
            if (isKeyword(tkn.lexeme))
                strcpy(tkn.type, "Keyword");
            else
                strcpy(tkn.type, "Identifier");
            gotToken = 1;
            fseek(fin, -1, SEEK_CUR);
        }

        // Process String Literals
        else if (c == '"') {
            tkn.row = row;
            tkn.col = col;
            strcpy(tkn.type, "StringLiteral");
            int k = 1;
            tkn.lexeme[0] = '"';
            while ((c = getc(fin)) != EOF && c != '"') {
                tkn.lexeme[k++] = c;
                ++col;
            }
            tkn.lexeme[k] = '"';
            gotToken = 1;
        }

        else
            ++col;
    }
    return tkn;
}