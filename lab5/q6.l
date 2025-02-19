%{
#include <stdio.h>
#include <ctype.h>

int yywrap();
int yywrap() {
    return 1; // Indicate end of input
}
%}

%%
"/*"([^*]|\*+[^/])*"*/"   { /* Ignore comments */ }
[a-zA-Z]      { if (isupper(yytext[0])) { yytext[0] = tolower(yytext[0]); } ECHO; }
.              { ECHO; }

%% 

int main(int argc, char **argv) {
    yylex();
    return 0;
}
