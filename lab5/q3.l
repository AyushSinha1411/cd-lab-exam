%{
#include <stdio.h>
int yywrap() ;
int positive_integers = 0;
int negative_integers = 0;
int positive_floats = 0;
int negative_floats = 0;

int yywrap() {
    return 1; // Indicate end of input
}
%}

%%
[+-]?[0-9]+        { if (yytext[0] == '-') negative_integers++; else positive_integers++; }
[+-]?[0-9]*\.[0-9]+ { if (yytext[0] == '-') negative_floats++; else positive_floats++; }

%% 

int main(int argc, char **argv) {
    positive_integers = 0;
    negative_integers = 0;
    positive_floats = 0;
    negative_floats = 0;
    yylex();
    printf("Positive Integers: %d, Negative Integers: %d, Positive Floats: %d, Negative Floats: %d\n", positive_integers, negative_integers, positive_floats, negative_floats);
    return 0;
}
