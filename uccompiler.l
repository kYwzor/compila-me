%X COMMENT
%{
    #define INVALID_CHARLIT 1
    #define UNTERM_COMMENT 2
    #define UNTERM_CHARLIT 3
    #define ILLEGAL_CHAR 4
    /*Pedro Carvalho 2015232484
    Tiago Gomes 2015238615*/
    #include "stdio.h"
    #include "ctype.h"
    int flag = 0;
    int line = 1;
    int column = 1;
    void handleError(int errorCode);
    void handleKeyword(char* keyword);
    void handleToken(char* token);
    //reserved ["auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"int"|"long"|"register"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"]
    //garantir reserved
    //\n, \t \\, \’, \" e \ooo
%}
numero                                              [0-9] 
decimal                                             \.|\,
reserved                                            \[|\]|\+\+|\-\-|auto|break|case|const|continue|default|do|enum|extern|float|for|goto|inline|long|register|restrict|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|_Bool|_Complex|_Imaginary
chrlitconst                                         \\([0-7][0-7][0-7]|n|t|\\|\'|\")|[^'|\n|\\]
terminator                                          \n|\r|\r\n
separator                                           \t|" "|\s|\b|\f
%%
\/\/.*                                              ;
({numero})+                                            {handleToken("INTLIT");}
(({numero})*({decimal})?({numero})+|({numero})+({decimal})?({numero})*)(e|E)?[\-|\+]?({numero})*       {handleToken("REALLIT");} 
'({chrlitconst})'                                    {handleToken("CHRLIT");}
'([^'|\n|\r|\r\n|\\]|(\\.)*)*'      {handleError(INVALID_CHARLIT);}
('\\)+                                              {handleError(UNTERM_CHARLIT);}
'([^'|\n|\r|\r\n|\\]|(\\.)*)+((\\\\)*\\')?({terminator})         {handleError(UNTERM_CHARLIT); line++; column = 1;}
\/\*                                                {BEGIN COMMENT; column+= yyleng;}
<COMMENT>.                                          {column += yyleng;}
<COMMENT>({terminator})                           {line++; column = 1;}
<COMMENT>\*\/                                       {BEGIN 0; column += yyleng;}
<COMMENT><<EOF>>                                    {handleError(UNTERM_COMMENT);}
char                                                {handleKeyword("CHAR");}
else                                                {handleKeyword("ELSE");}
while                                               {handleKeyword("WHILE");}
if                                                  {handleKeyword("IF");}
int                                                 {handleKeyword("INT");}
short                                               {handleKeyword("SHORT");}
double                                              {handleKeyword("DOUBLE");}
return                                              {handleKeyword("RETURN");}
void                                                {handleKeyword("VOID");}
\&                                                  {handleKeyword("BITWISEAND");}
\|                                                  {handleKeyword("BITWISEOR");}
\^                                                  {handleKeyword("BITWISEXOR");}
\&\&                                                {handleKeyword("AND");}
\=                                                  {handleKeyword("ASSIGN");}
\*                                                  {handleKeyword("MUL");}
\,                                                  {handleKeyword("COMMA");}
\/                                                  {handleKeyword("DIV");}
\=\=                                                {handleKeyword("EQ");}
\>\=                                                {handleKeyword("GE");}
\>                                                  {handleKeyword("GT");}
\{                                                  {handleKeyword("LBRACE");}
\<\=                                                {handleKeyword("LE");}
\(                                                  {handleKeyword("LPAR");}
\<                                                  {handleKeyword("LT");}
\-                                                  {handleKeyword("MINUS");}
\%                                                  {handleKeyword("MOD");}
\!\=                                                {handleKeyword("NE");}
\!                                                  {handleKeyword("NOT");}
\|\|                                                {handleKeyword("OR");}
\+                                                  {handleKeyword("PLUS");}
\}                                                  {handleKeyword("RBRACE");}
\)                                                  {handleKeyword("RPAR");}
\;                                                  {handleKeyword("SEMI");}
({reserved})                                          {handleToken("RESERVED");}
[a-z|A-Z|_][a-z|A-Z|_|0-9]*                         {handleToken("ID");}
({separator})                                         {column += yyleng;}
.                                                   {handleError(ILLEGAL_CHAR);}
({terminator})                                        {line++; column = 1;}

%%
int main(int argc, char** argv)
{
    if(argc == 2 && strcmp(argv[1],"-l") == 0) flag = 1;
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}

void handleKeyword(char* keyword){
    if(flag){
        printf("%s\n", keyword);
        column += yyleng;
    }
}

void handleToken(char* token){
    if(flag){
        printf("%s(%s)\n", token, yytext);
        column += yyleng;
    }
}

void handleError(int errorCode){
    if(errorCode == 1){
        printf("Line %d, col %d: invalid char constant (%s)\n", line, column, yytext);
    }
    else if(errorCode == 2){
        printf("Line %d, col %d: unterminated comment\n", line, column);
    }
    else if(errorCode == 3){
        printf("Line %d, col %d: unterminated char constant\n", line, column);
    }
    else if(errorCode == 4){
        printf("Line %d, col %d: illegal character (%s)\n", line, column,yytext);
    }
    column += yyleng;
}