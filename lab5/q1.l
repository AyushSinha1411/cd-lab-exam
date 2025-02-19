%{
#include <stdio.h>

int yywrap();

int vowels = 0;
int consonants = 0;

int yywrap() {
    return 1; // Indicate end of input
}
%}

%%
[aeiouAEIOU]   { vowels++; }
[a-zA-Z]       { consonants++; } // Only count alphabetic characters as consonants

%% 

int main(int argc, char **argv) {
    vowels = 0;
    consonants = 0;
    yylex();
    printf("Vowels: %d, Consonants: %d\n", vowels, consonants);
    return 0;
}

