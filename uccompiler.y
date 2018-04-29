%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "tree.h"
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
%right ASSIGN

%union{
    char *value;
    Node node;
 }

%type <node> Program Functions_and_declarations Functions_and_declarations_mandatory Functions_and_declarations_none_or_more Function_definition Function_body Declarations_and_statements Function_declaration Function_declarator Parameter_list Parameter_declaration Declaration Declarator_list Declarator_none_or_more Declarator Type_spec Statement Statement_or_error Statement_one_or_more Expr Assignment_expr Logical_OR_expr Logical_AND_expr Inclusive_OR_expr Exclusive_OR_expr AND_expr Equality_expr Relational_expr Additive_expr Multiplicative_expr Unary_expression Argument_expr_list Postfix_expr Primary_expr

%%
Program:
    Functions_and_declarations  {treeRoot = create_node(Program, NULL); add_child(treeRoot, $1);}
    ;    

Functions_and_declarations:
    Functions_and_declarations_mandatory Functions_and_declarations_none_or_more    {$$=$1; add_brother($1, $2);}
    ;

Functions_and_declarations_mandatory:
    Function_definition     {$$ = $1;}
    | Function_declaration  {$$ = $1;}
    | Declaration           {$$ = $1;}
    ;

Functions_and_declarations_none_or_more:
    Functions_and_declarations_mandatory Functions_and_declarations_none_or_more    {$$=$1; add_brother($1, $2);}
    |                                                                               {$$=NULL;}
    ;

Function_definition:
    Type_spec Function_declarator Function_body     {$$=create_node(FuncDefinition, NULL); add_child($$,$1); add_brother($1, $2); add_brother($1, $3);}
    ;

Function_body:
    LBRACE Declarations_and_statements RBRACE   {$$=create_node(FuncBody, NULL); add_child($$, $2);}
    | LBRACE RBRACE                             {$$=create_node(FuncBody, NULL);}
    ;

Declarations_and_statements:
    Declarations_and_statements Statement       {$$=$1; add_brother($1, $2);}
    | Declarations_and_statements Declaration   {$$=$1; add_brother($1, $2);}
    | Statement                                 {$$=$1;}
    | Declaration                               {$$=$1;}
    ;

Function_declaration:
    Type_spec Function_declarator SEMI  {$$=create_node(FuncDeclaration, NULL); add_child($$, $1); add_brother($1, $2);}
    ;

Function_declarator:
    ID LPAR Parameter_list RPAR {$$=create_node(Id, $1); aux = create_node(ParamList,NULL); add_brother($$, aux); add_child(aux, $3);}
    ;

Parameter_list:
    Parameter_declaration  {$$ = $1;}
    | Parameter_list COMMA Parameter_declaration {$$ = $1; add_brother($$, $3);}
    ;

Parameter_declaration:
    Type_spec ID    {$$=create_node(ParamDeclaration,NULL); add_child($$, $1); add_brother($1, create_node(Id, $2));}
    | Type_spec     {$$=create_node(ParamDeclaration,NULL); add_child($$, $1);}
    ;

Declaration:
    Type_spec Declarator_list SEMI  {add_brother($1, $2->child);
                                    $2->child = $1;
                                    aux = $2->brother;
                                    while(aux!=NULL){
                                        aux2 = create_node($1->label, NULL);
                                        add_brother(aux2, aux->child);
                                        aux->child = aux2;
                                        aux = aux->brother;
                                    }
                                    $$=$2;}
                                    
                                    
    | error SEMI {$$=create_node(Null, NULL);}
    ;

Declarator_list:
	Declarator Declarator_none_or_more {$$=create_node(Declaration, NULL); add_child($$, $1); add_brother($$, $2);}
	;

Declarator_none_or_more:
    COMMA Declarator_list   {$$=$2;}
    |                       {$$=NULL;}
    ;

Declarator:
    ID ASSIGN Expr              {$$ = create_node(Id, $1); add_brother($$, $3);}
    | ID                        {$$ = create_node(Id, $1);}
    ;

Type_spec: 
    INT         {$$=create_node(Int, NULL);}
    | CHAR      {$$=create_node(Char, NULL);}
    | VOID      {$$=create_node(Void, NULL);}
    | SHORT     {$$=create_node(Short, NULL);}
    | DOUBLE    {$$=create_node(Double, NULL);}
    ;

Statement:
    SEMI                                                                    {$$=NULL;}
    | Expr SEMI                                                             {$$=$1;}
    | LBRACE RBRACE                                                         {$$=NULL;}

    | LBRACE Statement_one_or_more RBRACE                                   {if ($2!=NULL && $2->brother!=NULL){
                                                                                $$ = create_node(StatList, NULL);
                                                                                add_child($$, $2);
                                                                            }
                                                                            else $$ = $2;}

    | LBRACE error RBRACE                                                   {$$=create_node(Null, NULL);}

    | IF LPAR Expr RPAR Statement_or_error ELSE Statement_or_error          {$$=create_node(If, NULL); add_child($$, $3);
                                                                            if ($5==NULL) add_brother($3, create_node(Null, NULL));
                                                                            else add_brother($3, $5);
                                                                            if ($7==NULL) add_brother($3, create_node(Null, NULL));
                                                                            else add_brother($3, $7);}

    | IF LPAR Expr RPAR Statement_or_error %prec NOELSE                     {$$=create_node(If, NULL); add_child($$, $3);
                                                                            if ($5==NULL) add_brother($3, create_node(Null, NULL));
                                                                            else add_brother($3, $5);
                                                                            add_brother($3, create_node(Null, NULL));}

    | WHILE LPAR Expr RPAR Statement_or_error                               {$$=create_node(While, NULL); add_child($$, $3);
                                                                            if ($5==NULL) add_brother($3, create_node(Null, NULL));
                                                                            else add_brother($3, $5);}

    | RETURN Expr SEMI                                                      {$$=create_node(Return, NULL); add_child($$, $2);}
    | RETURN SEMI                                                           {$$=create_node(Return, NULL); add_child($$, create_node(Null, NULL));}
    ;

Statement_or_error:
    Statement       {$$=$1;}
    | error SEMI    {$$=create_node(Null, NULL);}
    ;

Statement_one_or_more:
    Statement_one_or_more Statement_or_error    {$$=$1; add_brother($1, $2);}
    | Statement_or_error                        {$$=$1;}
    ;

Expr:
    Assignment_expr                 {$$ = $1;}
    | Expr COMMA Assignment_expr    {$$ = create_node(Comma, NULL); add_child($$, $1); add_brother($1, $3);}
    ;
    
Assignment_expr:
    Logical_OR_expr                             {$$ = $1;}
    | Logical_OR_expr ASSIGN Assignment_expr   {$$ = create_node(Store, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

Logical_OR_expr:
    Logical_AND_expr                        {$$ = $1;}
    | Logical_OR_expr OR Logical_AND_expr   {$$ = create_node(Or, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

Logical_AND_expr:
    Inclusive_OR_expr                           {$$ = $1;}
    | Logical_AND_expr AND Inclusive_OR_expr    {$$ = create_node(And, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

Inclusive_OR_expr:
    Exclusive_OR_expr                               {$$ = $1;}
    | Inclusive_OR_expr BITWISEOR Exclusive_OR_expr {$$ = create_node(BitWiseOr, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

Exclusive_OR_expr:
    AND_expr                                {$$ = $1;}
    | Exclusive_OR_expr BITWISEXOR AND_expr {$$ = create_node(BitWiseXor, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

AND_expr:
    Equality_expr                       {$$ = $1;}
    | AND_expr BITWISEAND Equality_expr {$$ = create_node(BitWiseAnd, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

Equality_expr:
    Relational_expr                     {$$ = $1;}
    | Equality_expr EQ Relational_expr  {$$ = create_node(Eq, NULL); add_child($$, $1); add_brother($1, $3);}
    | Equality_expr NE Relational_expr  {$$ = create_node(Ne, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

Relational_expr:
    Additive_expr                       {$$ = $1;}
    | Relational_expr LT Additive_expr  {$$ = create_node(Lt, NULL); add_child($$, $1); add_brother($1, $3);}
    | Relational_expr GT Additive_expr  {$$ = create_node(Gt, NULL); add_child($$, $1); add_brother($1, $3);}
    | Relational_expr LE Additive_expr  {$$ = create_node(Le, NULL); add_child($$, $1); add_brother($1, $3);}
    | Relational_expr GE Additive_expr  {$$ = create_node(Ge, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

Additive_expr:
    Multiplicative_expr                         {$$ = $1;}
    | Additive_expr PLUS Multiplicative_expr    {$$ = create_node(Add, NULL); add_child($$, $1); add_brother($1, $3);}
    | Additive_expr MINUS Multiplicative_expr   {$$ = create_node(Sub, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

Multiplicative_expr:
    Unary_expression                            {$$ = $1;}
    | Multiplicative_expr MUL Unary_expression  {$$ = create_node(Mul, NULL); add_child($$, $1); add_brother($1, $3);}
    | Multiplicative_expr DIV Unary_expression  {$$ = create_node(Div, NULL); add_child($$, $1); add_brother($1, $3);}
    | Multiplicative_expr MOD Unary_expression  {$$ = create_node(Mod, NULL); add_child($$, $1); add_brother($1, $3);}
    ;

Unary_expression:
    Postfix_expr                {$$ = $1;}
    | PLUS Unary_expression     {$$ = create_node(Plus, NULL); add_child($$, $2);}
    | MINUS Unary_expression    {$$ = create_node(Minus, NULL); add_child($$, $2);}
    | NOT Unary_expression      {$$ = create_node(Not, NULL); add_child($$, $2);}
    ;

Argument_expr_list:
    Assignment_expr                             {$$ = $1;}
    | Argument_expr_list COMMA Assignment_expr  {$$ = $1; add_brother($1, $3);}

Postfix_expr:
    Primary_expr                        {$$ = $1;}
    | ID LPAR RPAR                      {$$ = create_node(Call, NULL); add_child($$, create_node(Id, $1));}
    | ID LPAR Argument_expr_list RPAR   {$$ = create_node(Call, NULL); aux = create_node(Id, $1); add_child($$, aux); add_brother(aux, $3);}
    | ID LPAR error RPAR                {$$ = create_node(Null, NULL); free($1);}
    ;

Primary_expr:
    ID                  {$$ = create_node(Id, $1);}
    | INTLIT            {$$ = create_node(IntLit, $1);}
    | CHRLIT            {$$ = create_node(ChrLit, $1);}
    | REALLIT           {$$ = create_node(RealLit, $1);}
    | LPAR Expr RPAR    {$$ = $2;}
    | LPAR error RPAR   {$$ = create_node(Null, NULL);}
    ;

%%

void pprint(char* string){
    //printf("%s\n", string);
}


void yyerror (char *s) {
    if(flag!=1) printf("Line %d, col %d: %s: %s\n", line, column-(int)yyleng, s, yytext);
    errorFlag = 1;
}
