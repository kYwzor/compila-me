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
%type <node> Program Functions_and_declarations Functions_and_declarations_mandatory Functions_and_declarations_none_or_more Function_definition Function_body Declarations_and_statements Function_declaration Function_declarator Parameter_list Parameter_list_none_or_more Parameter_declaration Declaration Declarator_list Declarator_none_or_more Declarator Type_spec Expr Assignment_expr Logical_OR_expr Logical_AND_expr Inclusive_OR_expr Exclusive_OR_expr AND_expr Equality_expr Relational_expr Additive_expr Multiplicative_expr Unary_expression Postfix_expr Primary_expr

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
    Statement Declarations_and_statements       {$$=createNode("I don't understand statements", NULL); /*addChild($$, $1);*/ addBrother($$, $2);}
    | Declaration Declarations_and_statements   {$$=createNode("Declaration", NULL); addChild($$, $1); addBrother($$, $2);}
    | Statement                                 {$$=createNode("I don't understand statements", NULL); /*addChild($$, $1);*/}
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
    | error SEMI {;}
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
    | IF LPAR Expr RPAR Statement_or_error %prec NOELSE {;}
    | WHILE LPAR Expr RPAR Statement_or_error{;}
    | RETURN Expr SEMI {;}
    | RETURN SEMI {;}
    ;

Statement_or_error:
    Statement {;}
    | error SEMI {;}
    ;

Statement_none_or_more:
    Statement_or_error Statement_none_or_more {;}
    | {;}
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
    | Inclusive_OR_expr BITWISEOR Exclusive_OR_expr {$$ = createNode("BitwiseOr", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Exclusive_OR_expr:
    AND_expr {$$ = $1;}
    | Exclusive_OR_expr BITWISEXOR AND_expr {$$ = createNode("BitwiseXor", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

AND_expr:
    Equality_expr {$$ = $1;}
    | AND_expr BITWISEAND Equality_expr {$$ = createNode("BitwiseAnd", NULL); addChild($$, $1); addBrother($1, $3);}
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
    | PLUS Unary_expression {$$ = createNode("Plus", NULL); addChild($$, $1);}
    | MINUS Unary_expression {$$ = createNode("Minus", NULL); addChild($$, $1);}
    | NOT Unary_expression {$$ = createNode("Not", NULL); addChild($$, $1);}
    ;

Postfix_expr:
    Primary_expr {$$ = $1;}
    | ID LPAR Expr RPAR {$$ = createNode("Call", NULL); addChild($$, $3);}
    | ID LPAR RPAR  {$$ = createNode("Call", NULL); aux = createNode("Node", NULL); addChild($$, aux);}
    | ID LPAR error RPAR {$$ = createNode("Call", NULL); aux = createNode("Node", NULL); addChild($$, aux);}
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


void yyerror (char *s) {
    if(flag!=1) printf("Line %d, col %d: %s: %s\n", line, column-(int)yyleng, s, yytext);
}