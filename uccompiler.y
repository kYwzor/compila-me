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
    int yylex(void);
    void pprint(char* string);
    void yyerror (char *s);
%}



%token CHAR ELSE WHILE IF INT SHORT DOUBLE RETURN VOID BITWISEAND BITWISEOR BITWISEXOR AND ASSIGN MUL COMMA DIV EQ GE GT LBRACE LE LPAR LT MINUS MOD NE NOT OR PLUS RBRACE RPAR SEMI RESERVED
%token <value> REALLIT INTLIT CHRLIT ID

%nonassoc NOELSE
%nonassoc ELSE

%left COMMA
%nonassoc DECL

%union{
    char *value;
    Node node;
}

%type <node> Program Functions_and_declarations Functions_and_declarations_mandatory Functions_and_declarations_none_or_more Function_definition Function_body Declarations_and_statements Function_declaration Function_declarator Parameter_list Parameter_declaration Declaration Declarator_list Declarator_none_or_more Declarator Type_spec Statement Statement_or_error Statement_one_or_more Expr Assignment_expr Logical_OR_expr Logical_AND_expr Inclusive_OR_expr Exclusive_OR_expr AND_expr Equality_expr Relational_expr Additive_expr Multiplicative_expr Unary_expression Argument_expr_list Postfix_expr Primary_expr

%%
Program:
    Functions_and_declarations  {treeRoot = createNode("Program", NULL); addChild(treeRoot, $1);}
    ;    

Functions_and_declarations:
    Functions_and_declarations_mandatory Functions_and_declarations_none_or_more    {$$=$1; addBrother($1, $2);}
    ;

Functions_and_declarations_mandatory:
    Function_definition     {$$ = $1;}
    | Function_declaration  {$$ = $1;}
    | Declaration           {$$ = $1;}
    ;

Functions_and_declarations_none_or_more:
    Functions_and_declarations_mandatory Functions_and_declarations_none_or_more    {$$=$1; addBrother($1, $2);}
    |                                                                               {$$=NULL;}
    ;

Function_definition:
    Type_spec Function_declarator Function_body     {$$=createNode("FuncDefinition", NULL); addChild($$,$1); addBrother($1, $2); addBrother($1, $3);}
    ;

Function_body:
    LBRACE Declarations_and_statements RBRACE   {$$=createNode("FuncBody", NULL); addChild($$, $2);}
    | LBRACE RBRACE                             {$$=createNode("FuncBody", NULL);}
    ;

Declarations_and_statements:
    Declarations_and_statements Statement       {$$=$1; addBrother($1, $2);}
    | Declarations_and_statements Declaration   {$$=$1; addBrother($1, $2);}
    | Statement                                 {$$=$1;}
    | Declaration                               {$$=$1;}
    ;

Function_declaration:
    Type_spec Function_declarator SEMI  {$$=createNode("FuncDeclaration", NULL); addChild($$, $1); addBrother($1, $2);}
    ;

Function_declarator:
    ID LPAR Parameter_list RPAR {$$=createNode("Id", $1); aux = createNode("ParamList",NULL); addBrother($$, aux); addChild(aux, $3);}
    ;

Parameter_list:
    Parameter_declaration  {$$ = $1;}
    | Parameter_list COMMA Parameter_declaration {$$ = $1; addBrother($$, $3);}
    ;

Parameter_declaration:
    Type_spec ID    {$$=createNode("ParamDeclaration",NULL); addChild($$, $1); addBrother($1, createNode("Id", $2));}
    | Type_spec     {$$=createNode("ParamDeclaration",NULL); addChild($$, $1);}
    ;

Declaration:
    Type_spec Declarator_list SEMI  {addBrother($1, $2->child);
                                    $2->child = $1;
                                    aux = $2->brother;
                                    while(aux!=NULL){
                                        aux2 = createNode($1->label, NULL);
                                        addBrother(aux2, aux->child);
                                        aux->child = aux2;
                                        aux = aux->brother;
                                    }
                                    $$=$2;}
                                    
                                    
    | error SEMI {$$=createNode("Null", NULL);}
    ;

Declarator_list:
	Declarator Declarator_none_or_more {$$=createNode("Declaration", NULL); addChild($$, $1); addBrother($$, $2);}
	;

Declarator_none_or_more:
    COMMA Declarator_list   {$$=$2;}
    |                       {$$=NULL;}
    ;

Declarator:
    ID ASSIGN Expr %prec DECL   {$$ = createNode("Id", $1); addBrother($$, $3);}
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

    | LBRACE Statement_one_or_more RBRACE                                   {aux = $2;
                                                                            if (countBrothers($2)>=2){
                                                                                $$ = createNode("StatList", NULL);
                                                                                addChild($$, $2);
                                                                            }
                                                                            else $$ = $2;}

    | LBRACE error RBRACE                                                   {$$=createNode("Null", NULL);}

    | IF LPAR Expr RPAR Statement_or_error ELSE Statement_or_error          {$$=createNode("If", NULL); addChild($$, $3);
                                                                            if ($5==NULL) addBrother($3, createNode("Null", NULL));
                                                                            else addBrother($3, $5);
                                                                            if ($7==NULL) addBrother($3, createNode("Null", NULL));
                                                                            else addBrother($3, $7);}

    | IF LPAR Expr RPAR Statement_or_error %prec NOELSE                     {$$=createNode("If", NULL); addChild($$, $3);
                                                                            if ($5==NULL) addBrother($3, createNode("Null", NULL));
                                                                            else addBrother($3, $5);
                                                                            addBrother($3, createNode("Null", NULL));}

    | WHILE LPAR Expr RPAR Statement_or_error                               {$$=createNode("While", NULL); addChild($$, $3);
                                                                            if ($5==NULL) addBrother($3, createNode("Null", NULL));
                                                                            else addBrother($3, $5);}

    | RETURN Expr SEMI                                                      {$$=createNode("Return", NULL); addChild($$, $2);}
    | RETURN SEMI                                                           {$$=createNode("Return", NULL); addChild($$, createNode("Null", NULL));}
    ;

Statement_or_error:
    Statement       {$$=$1;}
    | error SEMI    {$$=createNode("Null", NULL);}
    ;

Statement_one_or_more:
    Statement_or_error Statement_one_or_more    {$$=$1; addBrother($1, $2);}
    | Statement_or_error                        {$$=$1;}
    ;

Expr:
    Assignment_expr                 {$$ = $1;}
    | Expr COMMA Assignment_expr    {$$ = createNode("Comma", NULL); addChild($$, $1); addBrother($1, $3);}
    ;
    
Assignment_expr:
    Logical_OR_expr                             {$$ = $1;}
    | Unary_expression ASSIGN Assignment_expr   {$$ = createNode("Store", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Logical_OR_expr:
    Logical_AND_expr                        {$$ = $1;}
    | Logical_OR_expr OR Logical_AND_expr   {$$ = createNode("Or", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Logical_AND_expr:
    Inclusive_OR_expr                           {$$ = $1;}
    | Logical_AND_expr AND Inclusive_OR_expr    {$$ = createNode("And", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Inclusive_OR_expr:
    Exclusive_OR_expr                               {$$ = $1;}
    | Inclusive_OR_expr BITWISEOR Exclusive_OR_expr {$$ = createNode("BitWiseOr", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Exclusive_OR_expr:
    AND_expr                                {$$ = $1;}
    | Exclusive_OR_expr BITWISEXOR AND_expr {$$ = createNode("BitWiseXor", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

AND_expr:
    Equality_expr                       {$$ = $1;}
    | AND_expr BITWISEAND Equality_expr {$$ = createNode("BitWiseAnd", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Equality_expr:
    Relational_expr                     {$$ = $1;}
    | Equality_expr EQ Relational_expr  {$$ = createNode("Eq", NULL); addChild($$, $1); addBrother($1, $3);}
    | Equality_expr NE Relational_expr  {$$ = createNode("Ne", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Relational_expr:
    Additive_expr                       {$$ = $1;}
    | Relational_expr LT Additive_expr  {$$ = createNode("Lt", NULL); addChild($$, $1); addBrother($1, $3);}
    | Relational_expr GT Additive_expr  {$$ = createNode("Gt", NULL); addChild($$, $1); addBrother($1, $3);}
    | Relational_expr LE Additive_expr  {$$ = createNode("Le", NULL); addChild($$, $1); addBrother($1, $3);}
    | Relational_expr GE Additive_expr  {$$ = createNode("Ge", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Additive_expr:
    Multiplicative_expr                         {$$ = $1;}
    | Additive_expr PLUS Multiplicative_expr    {$$ = createNode("Add", NULL); addChild($$, $1); addBrother($1, $3);}
    | Additive_expr MINUS Multiplicative_expr   {$$ = createNode("Sub", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Multiplicative_expr:
    Unary_expression                            {$$ = $1;}
    | Multiplicative_expr MUL Unary_expression  {$$ = createNode("Mul", NULL); addChild($$, $1); addBrother($1, $3);}
    | Multiplicative_expr DIV Unary_expression  {$$ = createNode("Div", NULL); addChild($$, $1); addBrother($1, $3);}
    | Multiplicative_expr MOD Unary_expression  {$$ = createNode("Mod", NULL); addChild($$, $1); addBrother($1, $3);}
    ;

Unary_expression:
    Postfix_expr                {$$ = $1;}
    | PLUS Unary_expression     {$$ = createNode("Plus", NULL); addChild($$, $2);}
    | MINUS Unary_expression    {$$ = createNode("Minus", NULL); addChild($$, $2);}
    | NOT Unary_expression      {$$ = createNode("Not", NULL); addChild($$, $2);}
    ;

Argument_expr_list:
    Assignment_expr                             {$$ = $1;}
    | Argument_expr_list COMMA Assignment_expr  {$$ = $1; addBrother($1, $3);}

Postfix_expr:
    Primary_expr                        {$$ = $1;}
    | ID LPAR RPAR                      {$$ = createNode("Call", NULL); addChild($$, createNode("Id", $1));}
    | ID LPAR Argument_expr_list RPAR   {$$ = createNode("Call", NULL); aux = createNode("Id", $1); addChild($$, aux); addBrother(aux, $3);}
    | ID LPAR error RPAR                {$$ = createNode("Null", NULL); free($1); /*maybe node null instead?*/}
    ;

Primary_expr:
    ID                  {$$ = createNode("Id", $1);}
    | INTLIT            {$$ = createNode("IntLit", $1);}
    | CHRLIT            {$$ = createNode("ChrLit", $1);}
    | REALLIT           {$$ = createNode("RealLit", $1);}
    | LPAR Expr RPAR    {$$ = $2;}
    | LPAR error RPAR   {$$ = createNode("Null", NULL);}
    ;

%%

void pprint(char* string){
    //printf("%s\n", string);
}


void yyerror (char *s) {
    if(flag!=1) printf("Line %d, col %d: %s: %s\n", line, column-(int)yyleng, s, yytext);
    errorFlag = 1;
}