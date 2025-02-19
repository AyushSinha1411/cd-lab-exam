%{
#include <stdio.h>

int words = 0;
int chars = 0;
int blanks = 0;
int lines = 0;
%}

%%

[a-zA-Z0-9]+  { words++; chars += yyleng; }  // Match a word, count characters in the word
[ \t]          { blanks++; chars++; }         // Match spaces and tabs, count as blanks and characters
\n             { lines++; chars++; }          // Match a newline, count lines and newline as a character
.              { chars++; }                   // Match any other character, count it as a character

%%

int yywrap() { return 1; }

int main() {
    printf("Enter the text (Ctrl+D to stop): \n");

    yylex();  // Start lexical analysis

    // Output the counts after analysis
    printf("Words: %d\n", words);
    printf("Characters: %d\n", chars);
    printf("Blanks: %d\n", blanks);
    printf("Lines: %d\n", lines);

    return 0;
}
