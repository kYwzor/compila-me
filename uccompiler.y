%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #define DEBUG 1
    int yylex(void);
    void pprint(char* string);
    void yyerror (const char *s);
%}

%token REALLIT  INTLIT  CHRLIT  CHAR  ELSE  WHILE  IF  INT  SHORT  DOUBLE  RETURN  VOID  BITWISEAND  BITWISEOR  BITWISEXOR  AND  ASSIGN  MUL  COMMA  DIV  EQ  GE  GT  LBRACE  LE  LPAR  LT  MINUS  MOD  NE  NOT  OR  PLUS  RBRACE  RPAR  SEMI  RESERVED  ID

/*Precendencia como definidas no C em si*/
%left LPAR  RPAR
 
%right NOT

%left MUL  DIV  MOD
%left PLUS  MINUS

%left GE  GT  LE  LT
%left EQ  NE

%left BITWISEAND
%left BITWISEXOR
%left BITWISEOR

%left AND
%left OR

%left COMMA

%union{
    char* value;
}

%%

functions_and_declarations:
    functions_and_declarations_mandatory functions_and_declarations_none_or_more
    ;

functions_and_declarations_mandatory:
    function_definition
    | function_declaration
    | declaration
    ;

functions_and_declarations_none_or_more:
    functions_and_declarations_mandatory functions_and_declarations_none_or_more
    | 
    ;

function_definition:
    type_spec function_declarator function_body
    ;

function_body:
    LBRACE declarations_and_statements RBRACE
    | LBRACE RBRACE
    ;

declarations_and_statements:
    statement declarations_and_statements
    | declaration declarations_and_statements
    | statement
    | declaration
    ;

function_declaration:
    type_spec function_declarator SEMI
    ;

function_declarator:
    ID LPAR parameter_list RPAR
    ;

parameter_list:
    parameter_declaration parameter_declaration_none_or_more 
    ;

parameter_declaration_none_or_more:
    COMMA parameter_declaration
    |
    ;

parameter_declaration:
    type_spec ID
    | type_spec
    ;

declaration:
    type_spec declarator declarator_none_or_more SEMI
    ;
    
declarator_none_or_more:
    COMMA declarator
    |
    ;

declarator:
    ID ASSIGN expr
    | ID
    ;

type_spec: 
    INT {pprint("Int");}
    | CHAR {pprint("Char");}
    | VOID {pprint("Void");}
    | SHORT {pprint("Short");}
    | DOUBLE {pprint("Double");}
    ;

statement:
    expr SEMI {;}
    | SEMI {;}
    | LBRACE statement_none_or_more RBRACE {;}
    | IF LPAR expr RPAR statement ELSE statement {;}
    | IF LPAR expr RPAR statement {;}
    | WHILE LPAR expr RPAR statement{;}
    | RETURN expr SEMI {;}
    | RETURN SEMI {;}
    ;

statement_none_or_more:
    statement statement_none_or_more {;}
    |
    ;

expr:
    assignment_expr {;}
    | expr COMMA assignment_expr {;}
    ;
    
assignment_expr:
    logical_OR_expr {;}
    | ASSIGN  assignment_expr {;}
    ;


logical_OR_expr:
    logical_AND_expr {;}
    | logical_OR_expr OR logical_AND_expr {;}
    ;

logical_AND_expr:
    inclusive_OR_expr {;}
    | logical_AND_expr AND inclusive_OR_expr {;}
    ;

inclusive_OR_expr:
    exclusive_OR_expr {;}
    | inclusive_OR_expr BITWISEOR exclusive_OR_expr {;}
    ;

exclusive_OR_expr:
    AND_expr {;}
    | exclusive_OR_expr BITWISEXOR AND_expr {;}
    ;

AND_expr:
    equality_expr {;}
    | AND_expr BITWISEAND equality_expr {;}
    ;

equality_expr:
    relational_expr {;}
    | equality_expr EQ relational_expr{;}
    | equality_expr NE relational_expr {;}
    ;

relational_expr:
    additive_expr {;}
    | relational_expr LT  additive_expr {;}
    | relational_expr GT additive_expr {;}
    | relational_expr LE additive_expr {;}
    | relational_expr GE additive_expr {;}
    ;

additive_expr:
    multiplicative_expr {;}
    | additive_expr PLUS multiplicative_expr {;}
    | additive_expr MINUS multiplicative_expr {;}
    ;

multiplicative_expr:
    unary_expression {;}
    | multiplicative_expr MUL unary_expression{;}
    | multiplicative_expr DIV unary_expression{;}
    | multiplicative_expr MOD unary_expression {;}
    ;


unary_expression:
    postfix_expr {;}
    | PLUS unary_expression {;}
    | MINUS unary_expression {;}
    | NOT unary_expression {;}
    ;



argument_expr_list:
    assignment_expr {;}
    | argument_expr_list COMMA assignment_expr {;}
    ;

postfix_expr:
    primary_expr {;}
    | postfix_expr LPAR argument_expr_list RPAR {;}
    | postfix_expr LPAR RPAR  {;}
    ;

primary_expr:
    ID {;}
    | INTLIT {;}
    | CHRLIT {;}
    | REALLIT {;}
    | LPAR expr RPAR {;}
    ;

%%

void pprint(char* string){
    //printf("%s\n", string);
}

