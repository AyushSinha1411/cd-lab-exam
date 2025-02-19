%{
#include <stdio.h>
int yywrap();
int scanf_count = 0;
int printf_count = 0;

int yywrap() {
    return 1; // Indicate end of input
}
%}

%%
scanf          { scanf_count++; ECHO; }
printf         { printf_count++; ECHO; }
.              { ECHO; }

%% 

int main(int argc, char **argv) {
    scanf_count = 0;
    printf_count = 0;
    yylex();
    printf("Number of scanf: %d, Number of printf: %d\n", scanf_count, printf_count);
    return 0;
}
