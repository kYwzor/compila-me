%X ITEMIZE ENUMERATE
%{
    #include "stdio.h"
    #include "ctype.h"
    int count = 0;
    int chapter = 0;
    int section = 0;
    //TODO: Fazer o minus dos numeros
%}
numero                                      [0-9]+
decimal                                     [\.\,]
%%
\-?{numero}                                           {printf("INTLIT(%s)\n", yytext);}
\-?{numero}({decimal}{numero})?e[\-|\+]{numero}       {printf("REALLIT(%s)\n", yytext);} 
'..+'                                                 {printf("ERROR CHRLIT: %s has more than one character\n", yytext);}
'\\(n|t|\\|\'|\")                                     {printf("CHRLIT(%s)\n", yytext);}
'\'|\\.'                                              {printf("ERROR CHRLIT: %s is ' or an invalid escape sequence\n", yytext);}
'.'                                                   {printf("CHRLIT(%s)\n", yytext);}
[a-z|A-Z|_][a-z|A-Z|_|0-9]*                             {printf("ID(%s)\n", yytext);}
char" "                                               {printf("CHAR\n");}
else" "                                               {printf("ELSE\n");}
while" "                                              {printf("WHILE\n");}
if" "                                                 {printf("IF\n");}
int" "                                                {printf("INT\n");}
short" "                                              {printf("SHORT\n");}
double" "                                             {printf("DOUBLE\n");}
return" "                                             {printf("RETURN\n");}
void" "                                               {printf("VOID\n");}
\&" "                                                 {printf("BITWISEAND\n");}
\|" "                                                 {printf("BITWISEOR\n");}
\^" "                                                 {printf("BITWISEXOR\n");}
\&\&                                                  {printf("AND\n");}
\=" "                                                 {printf("ASSIGN\n");}
\*                                                    {printf("MUL\n");}
\,                                                    {printf("COMMA\n");}
\/                                                    {printf("DIV\n");}
\=\=                                                  {printf("EQ\n");}
\>\=                                                  {printf("GE\n");}
\>                                                    {printf("GT\n");}
\{                                                    {printf("LBRACE\n");}
\<\=                                                  {printf("LE\n");}
\(                                                    {printf("LPAR\n");}
\<                                                    {printf("LT\n");}
\-                                                    {printf("MINUS\n");}
\%                                                    {printf("MOD\n");}
\!\=                                                  {printf("NE\n");}
\!                                                    {printf("NOT\n");}
\|\|" "                                               {printf("OR\n");}
\+                                                    {printf("PLUS\n");}
\}                                                    {printf("RBRACE\n");}
\)                                                    {printf("RPAR\n");}
\;                                                    {printf("SEMI\n");}
" "                                                   ;
.                                                     {printf("RENEGADE:%s\n", yytext);}
\n                                                    ;
%%
int main()
{
    yyin = fopen("in.c", "r"); 
    yylex();
    fclose(yyin);
    fclose(yyout);
    return 0;
}

int yywrap()
{
    return 1;
}

