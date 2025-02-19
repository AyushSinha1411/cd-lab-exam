%{
#include <stdio.h>
int yywrap();
int yywrap() {
    return 1; // Indicate end of input
}
%}

%%
[0-9]+        { printf("Hexadecimal: %X\n", atoi(yytext)); }

%% 

int main(int argc, char **argv) {
    yylex();
    return 0;
}
