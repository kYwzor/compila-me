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
    int line_memory;
    int column_memory;
    void handleError(int errorCode);
    int handleKeyword(char* keyword);
    int handleToken(char* token_val);
    //reserved ["auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"int"|"long"|"register"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"]
    //garantir reserved
    //\n, \t \\, \’, \" e \ooo
%}
numero                                              [0-9] 
reserved                                            \[|\]|\+\+|\-\-|auto|break|case|const|continue|default|do|enum|extern|float|for|goto|inline|long|register|restrict|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|_Bool|_Complex|_Imaginary
chrlitconst                                         \\([0-7][0-7]?[0-7]?|n|t|\\|\'|\")|[^'\n\\]
terminator                                          \r\n|\n|\r
separator                                           \t|" "|\s|\b|\f
%%
\/\/.*                                              ;
({numero})+                                         {handleToken("INTLIT"); return INTLIT;}
(({numero})*\.?({numero})+|({numero})+\.?({numero})*)((e|E)[\-\+]?({numero})+)?       {handleToken("REALLIT"); return REALLIT;} 
'({chrlitconst})'                                   {handleToken("CHRLIT"); return CHRLIT;}
'([^'\n\r\\]|(\\.))*'                               {handleError(INVALID_CHARLIT);}
'([^'\n\r\\]|(\\.))*\\?                             {handleError(UNTERM_CHARLIT);}
\/\*                                                {BEGIN COMMENT; 
                                                    column_memory = column; 
                                                    line_memory = line; 
                                                    column_memory += yyleng;}
<COMMENT>({terminator})                             {line_memory += 1;
                                                    column_memory = 1;}
<COMMENT>.                                          {column_memory += yyleng;}
<COMMENT>\*\/                                       {BEGIN 0; 
                                                    line = line_memory;
                                                    column_memory += yyleng;
                                                    column = column_memory;}
<COMMENT><<EOF>>                                    {handleError(UNTERM_COMMENT); 
                                                    column = column_memory; 
                                                    line = line_memory; 
                                                    BEGIN 0;}
char                                                {handleKeyword("CHAR"); return CHAR;}
else                                                {handleKeyword("ELSE"); return ELSE;}
while                                               {handleKeyword("WHILE"); return WHILE;}
if                                                  {handleKeyword("IF"); return IF;}
int                                                 {handleKeyword("INT"); return INT;}
short                                               {handleKeyword("SHORT"); return SHORT;}
double                                              {handleKeyword("DOUBLE"); return DOUBLE;}
return                                              {handleKeyword("RETURN"); return RETURN;}
void                                                {handleKeyword("VOID"); return VOID;}
\&                                                  {handleKeyword("BITWISEAND"); return BITWISEAND;}
\|                                                  {handleKeyword("BITWISEOR"); return BITWISEOR;}
\^                                                  {handleKeyword("BITWISEXOR"); return BITWISEXOR;}
\&\&                                                {handleKeyword("AND"); return AND;}
\=                                                  {handleKeyword("ASSIGN"); return ASSIGN;}
\*                                                  {handleKeyword("MUL"); return MUL;}
\,                                                  {handleKeyword("COMMA"); return COMMA;}
\/                                                  {handleKeyword("DIV"); return DIV;}
\=\=                                                {handleKeyword("EQ"); return EQ;}
\>\=                                                {handleKeyword("GE"); return GE;}
\>                                                  {handleKeyword("GT"); return GT;}
\{                                                  {handleKeyword("LBRACE"); return LBRACE;}
\<\=                                                {handleKeyword("LE"); return LE;}
\(                                                  {handleKeyword("LPAR"); return LPAR;}
\<                                                  {handleKeyword("LT"); return LT;} 
\-                                                  {handleKeyword("MINUS"); return MINUS;}
\%                                                  {handleKeyword("MOD"); return MOD;}
\!\=                                                {handleKeyword("NE"); return NE;}
\!                                                  {handleKeyword("NOT"); return NOT;}
\|\|                                                {handleKeyword("OR"); return OR;}
\+                                                  {handleKeyword("PLUS"); return PLUS;}
\}                                                  {handleKeyword("RBRACE"); return RBRACE;}
\)                                                  {handleKeyword("RPAR"); return RPAR;}
\;                                                  {handleKeyword("SEMI"); return SEMI;}
({reserved})                                        {handleToken("RESERVED"); return RESERVED;}
[a-zA-Z_][a-zA-Z_0-9]*                              {handleToken("ID"); return ID;}
({separator})                                       {column += yyleng;}
({terminator})                                      {line++; column = 1;}
.                                                   {handleError(ILLEGAL_CHAR);}

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

int handleKeyword(char* keyword){
    if(flag){
        printf("%s\n", keyword);
    }
    column += yyleng;
    return token;
}

int handleToken(char* token_val){
    if(flag){
        printf("%s(%s)\n", token_val, yytext);
    }
    column += yyleng;
    return token;
}

int handleError(int errorCode){
    if(errorCode == 1){
        printf("Line %d, col %d: invalid char constant (%s)\n", line, column, yytext);
        column += yyleng;
    }
    else if(errorCode == 2){
        printf("Line %d, col %d: unterminated comment\n", line, column);
        column_memory += yyleng;
    }
    else if(errorCode == 3){
        printf("Line %d, col %d: unterminated char constant\n", line, column);
        column += yyleng;
    }
    else if(errorCode == 4){
        printf("Line %d, col %d: illegal character (%s)\n", line, column, yytext);
        column += yyleng;
    }
}

void yyerror (char *s){
    printf ("%s\n", s);
}
