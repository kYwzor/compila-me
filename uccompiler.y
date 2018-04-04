%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "tree.h"
    #define DEBUG 1
    extern int line;
    extern int yyleng;
    extern int column;
    extern char* yytext;
    extern char flag;
    int yylex(void);
    void pprint(char* string);
    void yyerror (char *s);
%}



%token CHAR ELSE WHILE IF INT SHORT DOUBLE RETURN VOID BITWISEAND BITWISEOR BITWISEXOR AND ASSIGN MUL COMMA DIV EQ GE GT LBRACE LE LPAR LT MINUS MOD NE NOT OR PLUS RBRACE RPAR SEMI RESERVED

%nonassoc NOELSE
%nonassoc ELSE

%left COMMA
%nonassoc DECL

%union{
    char *value;
    Node node;
}

%token <value> REALLIT INTLIT CHRLIT ID
%type <node> Program Functions_and_declarations Functions_and_declarations_mandatory Functions_and_declarations_none_or_more Function_definition Function_body Declarations_and_statements Function_declaration Function_declarator Parameter_list Parameter_list_none_or_more Parameter_declaration Declaration Declarator_list Declarator_none_or_more Declarator Type_spec Statement Statement_or_error Statement_one_or_more Expr Assignment_expr Logical_OR_expr Logical_AND_expr Inclusive_OR_expr Exclusive_OR_expr AND_expr Equality_expr Relational_expr Additive_expr Multiplicative_expr Unary_expression Postfix_expr Primary_expr

%%
Program:
    Functions_and_declarations  {treeRoot = createNode("Program", NULL); addChild(treeRoot, $1);}
    ;    

Functions_and_declarations:
    Functions_and_declarations_mandatory Functions_and_declarations_none_or_more    {$$=$1; addBrother($1, $2);}
    ;

Functions_and_declarations_mandatory:
    Function_definition     {$$ = createNode("FuncDefinition", NULL); addChild($$, $1);}
    | Function_declaration  {$$ = createNode("FuncDeclaration", NULL); addChild($$, $1);}
    | Declaration           {$$ = createNode("Declaration", NULL); addChild($$, $1);}
    ;

Functions_and_declarations_none_or_more:
    Functions_and_declarations_mandatory Functions_and_declarations_none_or_more    {$$=$1; addBrother($1, $2);}
    |                                                                               {$$=NULL;}
    ;

Function_definition:
    Type_spec Function_declarator Function_body     {$$=$1; addBrother($1, $2);
                                                    aux = createNode("FuncBody", NULL); addChild(aux, $3);
                                                    addBrother($2, aux);}
    ;

Function_body:
    LBRACE Declarations_and_statements RBRACE   {$$=$2;}
    | LBRACE RBRACE                             {$$=NULL;}
    ;

Declarations_and_statements:
    Statement Declarations_and_statements       {$$=$1; addBrother($$, $2);}
    | Declaration Declarations_and_statements   {$$=createNode("Declaration", NULL); addChild($$, $1); addBrother($$, $2);}
    | Statement                                 {$$=$1;}
    | Declaration                               {$$=createNode("Declaration", NULL); addChild($$, $1);}
    ;

Function_declaration:
    Type_spec Function_declarator SEMI  {$$=$1; addBrother($1, $2);}
    ;

Function_declarator:
    ID LPAR Parameter_list RPAR {$$=createNode("Id", $1);
                                aux = createNode("ParamList", NULL); addChild(aux, $3);
                                addBrother($$, aux);}
    ;

Parameter_list:
    Parameter_declaration Parameter_list_none_or_more   {$$ = createNode("ParamDeclaration", NULL); addChild($$, $1); addBrother($$, $2);}
    ;

Parameter_list_none_or_more:
    COMMA Parameter_list    {$$ = $2;}
    |                       {$$ = NULL;}
    ;

Parameter_declaration:
    Type_spec ID    {$$=$1; addBrother($1, createNode("Id", $2));}
    | Type_spec     {$$=$1;}
    ;

Declaration:
    Type_spec Declarator_list SEMI  {$$=$1; addBrother($1, $2);}
    | error SEMI {$$=createNode("Null", NULL);}
    ;

Declarator_list:
	Declarator Declarator_none_or_more {$$=$1; addBrother($1, $2);}
	;

Declarator_none_or_more:
    COMMA Declarator_list   {$$=$2;}
    |                       {$$=NULL;}
    ;

Declarator:
    ID ASSIGN Assignment_expr   {$$ = createNode("Store", NULL); aux = createNode("Id", $1); addChild($$, aux); addBrother(aux, $3);}
    | ID                        {$$ = createNode("Id", $1);}
    ;

Type_spec: 
    INT         {$$=createNode("Int", NULL);}
    | CHAR      {$$=createNode("Char", NULL);}
    | VOID      {$$=createNode("Void", NULL);}
    | SHORT     {$$=createNode("Short", NULL);}
    | DOUBLE    {$$=createNode("Double", NULL);}
    ;

Statement:
    SEMI                                                                    {$$=NULL;}
    | Expr SEMI                                                             {$$=$1;}
    | LBRACE RBRACE                                                         {$$=NULL;}
    | LBRACE Statement_one_or_more RBRACE                                   {$$=$2;}
    | LBRACE error RBRACE                                                   {$$=createNode("Null", NULL);}
    | IF LPAR Expr RPAR Statement_or_error ELSE Statement_or_error          {$$=createNode("If", NULL); addChild($$, $3); addBrother($3, $5); addBrother($5, $7);}
    | IF LPAR Expr RPAR Statement_or_error %prec NOELSE                     {$$=createNode("If", NULL); addChild($$, $3); addBrother($3, $5); addBrother($5, createNode("Null", NULL));}
    | WHILE LPAR Expr RPAR Statement_or_error                               {$$=createNode("While", NULL); addChild($$, $3); addBrother($3, $5);}
    | RETURN Expr SEMI                                                      {$$=createNode("Return", NULL); addChild($$, $2);}
    | RETURN SEMI                                                           {$$=createNode("Return", NULL); addChild($$, createNode("Null", NULL));}
    ;

Statement_or_error:
    Statement       {$$=$1;}
    | error SEMI    {$$=createNode("Null", NULL);}
    ;

Statement_one_or_more:
    Statement_or_error Statement_one_or_more    {$$=createNode("StatList", NULL); addChild($$, $1);
                                                if (strcmp("StatList", $2->label) == 0){addBrother($1, $2->child); free($2->label); free($2);}
                                                else addBrother($1, $2);}
    | Statement_or_error                        {$$=$1;}
    ;

Expr:
    Assignment_expr {$$ = $1;}
    | Expr COMMA Assignment_expr {$$ = createNode("Comma", NULL); addChild($$, $1); addBrother($1, $3);}
    ;
    
Assignment_expr:
    Logical_OR_expr {$$ = $1;}
    | Unary_expression ASSIGN Assignment_expr {$$ = createNode("Store", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Logical_OR_expr:
    Logical_AND_expr {$$ = $1;}
    | Logical_OR_expr OR Logical_AND_expr {$$ = createNode("Or", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Logical_AND_expr:
    Inclusive_OR_expr {$$ = $1;}
    | Logical_AND_expr AND Inclusive_OR_expr {$$ = createNode("And", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Inclusive_OR_expr:
    Exclusive_OR_expr {$$ = $1;}
    | Inclusive_OR_expr BITWISEOR Exclusive_OR_expr {$$ = createNode("BitWiseOr", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Exclusive_OR_expr:
    AND_expr {$$ = $1;}
    | Exclusive_OR_expr BITWISEXOR AND_expr {$$ = createNode("BitWiseXor", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

AND_expr:
    Equality_expr {$$ = $1;}
    | AND_expr BITWISEAND Equality_expr {$$ = createNode("BitWiseAnd", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Equality_expr:
    Relational_expr {$$ = $1;}
    | Equality_expr EQ Relational_expr {$$ = createNode("Eq", NULL); addChild($$, $1); addBrother($1, $3);}
    | Equality_expr NE Relational_expr {$$ = createNode("Ne", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Relational_expr:
    Additive_expr {$$ = $1;}
    | Relational_expr LT Additive_expr {$$ = createNode("Lt", NULL); addChild($$, $1); addBrother($1, $3);}
    | Relational_expr GT Additive_expr {$$ = createNode("Gt", NULL); addChild($$, $1); addBrother($1, $3);}
    | Relational_expr LE Additive_expr {$$ = createNode("Le", NULL); addChild($$, $1); addBrother($1, $3);}
    | Relational_expr GE Additive_expr {$$ = createNode("Ge", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Additive_expr:
    Multiplicative_expr {$$ = $1;}
    | Additive_expr PLUS Multiplicative_expr {$$ = createNode("Add", NULL); addChild($$, $1); addBrother($1, $3);}
    | Additive_expr MINUS Multiplicative_expr {$$ = createNode("Sub", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Multiplicative_expr:
    Unary_expression {$$ = $1;}
    | Multiplicative_expr MUL Unary_expression {$$ = createNode("Mul", NULL); addChild($$, $1); addBrother($1, $3);}
    | Multiplicative_expr DIV Unary_expression {$$ = createNode("Div", NULL); addChild($$, $1); addBrother($1, $3);}
    | Multiplicative_expr MOD Unary_expression {$$ = createNode("Mod", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Unary_expression:
    Postfix_expr {$$ = $1;}
    | PLUS Unary_expression {$$ = createNode("Plus", NULL); addChild($$, $2);}
    | MINUS Unary_expression {$$ = createNode("Minus", NULL); addChild($$, $2);}
    | NOT Unary_expression {$$ = createNode("Not", NULL); addChild($$, $2);}
    ;

Postfix_expr:
    Primary_expr {$$ = $1;}
    | ID LPAR Expr RPAR {$$ = createNode("Call", NULL); addChild($$, $3);}
    | ID LPAR RPAR  {$$ = createNode("Call", NULL); aux = createNode("Null", NULL); addChild($$, aux);}
    | ID LPAR error RPAR {$$ = createNode("Call", NULL); aux = createNode("Null", NULL); addChild($$, aux);}
    ;

Primary_expr:
    ID {$$ = createNode("Id", $1);}
    | INTLIT {$$ = createNode("IntLit", $1);}
    | CHRLIT {$$ = createNode("ChrLit", $1);}
    | REALLIT {$$ = createNode("RealLit", $1);}
    | LPAR Expr RPAR {$$ = $2;}
    | LPAR error RPAR {$$ = createNode("Null", NULL);}
    ;

%%

void pprint(char* string){
    //printf("%s\n", string);
}


void yyerror (char *s) {
    if(flag!=1) printf("Line %d, col %d: %s: %s\n", line, column-(int)yyleng, s, yytext);
}