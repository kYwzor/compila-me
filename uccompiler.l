%X COMMENT
%{
    #define INVALID_CHARLIT 1
    #define UNTERM_COMMENT 2
    #define UNTERM_CHARLIT 3
    #define ILLEGAL_CHAR 4
    /*Pedro Carvalho 2015232484
    Tiago Gomes 2015238615*/
    #include "tree.h"
    #include "y.tab.h"
    int line = 1;
    int column = 1;
    int line_memory;
    int column_memory;
    void handleError(int errorCode);
    void handleKeyword(char* keyword);
    void handleToken(char* token_val);
%}

%option nounput
%option noinput

numero                                              [0-9] 
reserved                                            \[|\]|\+\+|\-\-|auto|break|case|const|continue|default|do|enum|extern|float|for|goto|inline|long|register|restrict|signed|sizeof|static|struct|switch|typedef|union|unsigned|volatile|_Bool|_Complex|_Imaginary
chrlitconst                                         \\([0-7][0-7]?[0-7]?|n|t|\\|\'|\")|[^'\n\\]
terminator                                          \r\n|\n|\r
separator                                           \t|" "|\v|\f
%%
\/\/.*                                              {column+=yyleng;};
({numero})+                                         {handleToken("INTLIT"); yylval.value = strdup(yytext); return INTLIT;}
(({numero})*\.?({numero})+|({numero})+\.?({numero})*)((e|E)[\-\+]?({numero})+)?       {handleToken("REALLIT"); yylval.value = strdup(yytext); return REALLIT;} 
'({chrlitconst})'                                   {handleToken("CHRLIT"); yylval.value = strdup(yytext); return CHRLIT;}
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
                                                    column = column_memory; line = line_memory; BEGIN 0;} 
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
[a-zA-Z_][a-zA-Z_0-9]*                              {handleToken("ID"); yylval.value = strdup(yytext); return ID;}
({separator})                                       {column += yyleng;}
({terminator})                                      {line++; column = 1;}
<<EOF>>                                             {column++; return 0;}
.                                                   {handleError(ILLEGAL_CHAR);}

%%
int main(int argc, char** argv)
{
    flag = 0;   // LLVM
    errorFlag = 0;
    treeRoot = NULL;
    if(argc == 2){
        if (strcmp(argv[1],"-1") == 0) flag = 1;        // lex errors
        else if (strcmp(argv[1],"-l") == 0) flag = 2;   // tokens + lex errors
        else if (strcmp(argv[1],"-2") == 0) flag = 3;   // syn and lex errors
        else if (strcmp(argv[1],"-t") == 0) flag = 4;   // syn and lex errors + AST (if no syn errors)
        else if (strcmp(argv[1],"-3") == 0) flag = 5;   // sem errors + syn and lex errors
        else if (strcmp(argv[1],"-s") == 0) flag = 6;   // sem errors + table + annotated AST + syn and lex errors
    }
    yyparse();
    if (treeRoot == NULL){
        return 0;
    }
    // if (flag < 4) nothing to do here
    if(!errorFlag){
        if (flag == 4) print_tree(treeRoot, 0);
        else if(flag == 5) handle_node(treeRoot);
        else if(flag == 6) {
            handle_node(treeRoot);
            print_tables();
            print_tree(treeRoot, 0);
        }
        else if(flag == 0){
            handle_node(treeRoot);
            generate_code(treeRoot);
        }
    }

    free_tree(treeRoot);
    return 0;
}

int yywrap()
{
    return 1;
}

void handleKeyword(char* keyword){
    if(flag==2){
        printf("%s\n", keyword);
    }
    column += yyleng;
}

void handleToken(char* token_val){
    if(flag==2){
        printf("%s(%s)\n", token_val, yytext);
    }
    column += yyleng;
}

void handleError(int errorCode){
    if(errorCode == INVALID_CHARLIT){
        printf("Line %d, col %d: invalid char constant (%s)\n", line, column, yytext);
        column += yyleng;
    }
    else if(errorCode == UNTERM_COMMENT){
        printf("Line %d, col %d: unterminated comment\n", line, column);
        column_memory += yyleng;
    }
    else if(errorCode == UNTERM_CHARLIT){
        printf("Line %d, col %d: unterminated char constant\n", line, column);
        column += yyleng;
    }
    else if(errorCode == ILLEGAL_CHAR){
        printf("Line %d, col %d: illegal character (%s)\n", line, column, yytext);
        column += yyleng;
    }
}
