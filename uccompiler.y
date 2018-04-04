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
    void yyerror (const char *s);
%}



%token CHAR ELSE WHILE IF INT SHORT DOUBLE RETURN VOID BITWISEAND BITWISEOR BITWISEXOR AND ASSIGN MUL COMMA DIV EQ GE GT LBRACE LE LPAR LT MINUS MOD NE NOT OR PLUS RBRACE RPAR SEMI RESERVED

%nonassoc RPAR
%nonassoc ELSE

%union{
    char *value;
    Node node;
}

%token <value> REALLIT INTLIT CHRLIT ID
%type <node> Program Functions_and_declarations Functions_and_declarations_mandatory Functions_and_declarations_none_or_more Function_definition Function_body Function_declaration Parameter_declaration Declaration Type_spec Function_declarator

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
    | Declaration           {$$ = createNode("FuncDefinition", NULL); addChild($$, $1);}
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
    LBRACE Declarations_and_statements RBRACE   {$$=createNode("temporary delete later", NULL); /*$$=$2;*/}
    | LBRACE RBRACE                             {$$=NULL;}
    ;

Declarations_and_statements:
    Statement Declarations_and_statements
    | Declaration Declarations_and_statements
    | Statement
    | Declaration
    ;

Function_declaration:
    Type_spec Function_declarator SEMI  {$$=createNode("temporary delete later", NULL);}
    ;

Function_declarator:
    ID LPAR Parameter_list RPAR {$$=createNode("temporary delete later", NULL);}
    ;

Parameter_list:
    Parameter_declaration Parameter_list_none_or_more 
    ;

Parameter_list_none_or_more:
    COMMA Parameter_list
    |
    ;

Parameter_declaration:
    Type_spec ID    {$$=createNode("temporary delete later", NULL);}
    | Type_spec     {$$=createNode("temporary delete later", NULL);}
    ;

Declaration:
    Type_spec Declarator_list SEMI  {$$=createNode("temporary delete later", NULL);}
    | error SEMI {;}
    ;

Declarator_list:
	Declarator Declarator_none_or_more
	;

Declarator_none_or_more:
    COMMA Declarator_list
    |
    ;

Declarator:
    ID ASSIGN Assignment_expr
    | ID
    ;

Type_spec: 
    INT {$$=createNode("Int", NULL);}
    | CHAR {$$=createNode("Char", NULL);}
    | VOID {$$=createNode("Void", NULL);}
    | SHORT {$$=createNode("Short", NULL);}
    | DOUBLE {$$=createNode("Double", NULL);}
    ;

Statement:
    SEMI {;}
    | Expr SEMI {;}
    | LBRACE Statement_none_or_more RBRACE {;}
    | LBRACE error RBRACE {;}
    | IF LPAR Expr RPAR Statement_or_error ELSE Statement_or_error {;}
    | IF LPAR Expr RPAR Statement_or_error {;}
    | WHILE LPAR Expr RPAR Statement_or_error{;}
    | RETURN Expr SEMI {;}
    | RETURN SEMI {;}
    ;

Statement_or_error:
    Statement {;}
    | error SEMI {;}
    ;

Statement_none_or_more:
    Statement Statement_none_or_more {;}
    |
    ;

Expr:
    Assignment_expr {;}
    | Expr COMMA Assignment_expr {;}
    ;
    
Assignment_expr:
    Logical_OR_expr {;}
    | Unary_expression ASSIGN Assignment_expr {;}
    ;

Logical_OR_expr:
    Logical_AND_expr {;}
    | Logical_OR_expr OR Logical_AND_expr {;}
    ;

Logical_AND_expr:
    Inclusive_OR_expr {;}
    | Logical_AND_expr AND Inclusive_OR_expr {;}
    ;

Inclusive_OR_expr:
    Exclusive_OR_expr {;}
    | Inclusive_OR_expr BITWISEOR Exclusive_OR_expr {;}
    ;

Exclusive_OR_expr:
    AND_expr {;}
    | Exclusive_OR_expr BITWISEXOR AND_expr {;}
    ;

AND_expr:
    Equality_expr {;}
    | AND_expr BITWISEAND Equality_expr {;}
    ;

Equality_expr:
    Relational_expr {;}
    | Equality_expr EQ Relational_expr {;}
    | Equality_expr NE Relational_expr {;}
    ;

Relational_expr:
    Additive_expr {;}
    | Relational_expr LT Additive_expr {;}
    | Relational_expr GT Additive_expr {;}
    | Relational_expr LE Additive_expr {;}
    | Relational_expr GE Additive_expr {;}
    ;

Additive_expr:
    Multiplicative_expr {;}
    | Additive_expr PLUS Multiplicative_expr {;}
    | Additive_expr MINUS Multiplicative_expr {;}
    ;

Multiplicative_expr:
    Unary_expression {;}
    | Multiplicative_expr MUL Unary_expression {;}
    | Multiplicative_expr DIV Unary_expression {;}
    | Multiplicative_expr MOD Unary_expression {;}
    ;

Unary_expression:
    Postfix_expr {;}
    | PLUS Unary_expression {;}
    | MINUS Unary_expression {;}
    | NOT Unary_expression {;}
    ;

Argument_expr_list:
    Assignment_expr {;}
    | Argument_expr_list COMMA Assignment_expr {;}
    ;

Postfix_expr:
    Primary_expr {;}
    | Postfix_expr LPAR Argument_expr_list RPAR {;}
    | Postfix_expr LPAR RPAR  {;}
    ;

Primary_expr:
    ID {;}
    | INTLIT {;}
    | CHRLIT {;}
    | REALLIT {;}
    | LPAR Expr RPAR {;}
    | LPAR error RPAR {;}
    ;

%%

void pprint(char* string){
    //printf("%s\n", string);
}


void yyerror (const char *s) {
    if(flag!=1) printf("Line %d, col %d: %s: %s\n", line, column-(int)yyleng, s, yytext);
}