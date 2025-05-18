tokens.l

%{
#include <stdio.h>
%}

%%
"int"           { printf("Keyword: int\n"); }
"float"         { printf("Keyword: float\n"); }
"return"        { printf("Keyword: return\n"); }
[0-9]+\.[0-9]+  { printf("Float constant: %s\n", yytext); }
[0-9]+          { printf("Integer constant: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]* { printf("Identifier: %s\n", yytext); }
"="             { printf("Operator: =\n"); }
"+"             { printf("Operator: +\n"); }
"{"             { printf("Symbol: {\n"); }
"}"             { printf("Symbol: }\n"); }
";"             { printf("Symbol: ;\n"); }
[ \t\n]+        { /* Ignore whitespace */ }
.               { printf("Unknown character: %s\n", yytext); }
%%

int main() {
    yylex();
    return 0;
}

int yywrap()
{
return 1;
}
    
                

int main() {
    int a = 10;
    float b = 20.5;
    a = a + b;
    return 0;
}
        


Commands:

lex tokens.l             # Generates lex.yy.c
gcc lex.yy.c -o lexer # Compile the lexer
./lexer < input.c         # Run it with input