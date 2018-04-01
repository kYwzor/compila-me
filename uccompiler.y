%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #define DEBUG 1
    int yylex(void);
    void pprint(char* string);
    void yyerror (const char *s);
%}

%token REALLIT, INTLIT, CHRLIT, CHAR, ELSE, WHILE, IF, INT, SHORT, DOUBLE, RETURN, VOID, BITWISEAND, BITWISEOR, BITWISEXOR, AND, ASSIGN, MUL, COMMA, DIV, EQ, GE, GT, LBRACE, LE, LPAR, LT, MINUS, MOD, NE, NOT, OR, PLUS, RBRACE, RPAR, SEMI, RESERVED, ID

/*Precendencia como definidas no C em si*/
%left LPAR, RPAR
 
%right NOT

%left MUL, DIV, MOD
%left PLUS, MINUS

%left GE, GT, LE, LT
%left EQ, NE

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
    statement statement_none_or_more 
    |
    ;

expr:
    expr expr_assignments %prec assign_precedence expr {pprint("expr_assignments");}
    | expr expr_math %prec math_precedence expr {pprint("expr_math");}
    | expr expr_logic %prec logic_precedence expr {pprint("expr_logic");}
    | expr expr_compare %prec compare_precedence expr {pprint("expr_compare");}
    | expr_modifier %prec modifier_precedence expr {pprint("expr_modifier");}
    | ID LPAR RPAR {pprint("expr_arguments");}
    | ID LPAR expr expr_arguments %prec arguments_precedence RPAR {pprint("expr_arguments");}
    | expr_values {pprint("expr_values");}
    ;


expr_assignments:
    ASSIGN 
    | COMMA
    ;

expr_math:
    PLUS
    | MINUS
    | MUL
    | DIV
    | MOD
    ;

expr_logic:
    OR
    | AND
    | BITWISEAND
    | BITWISEOR
    | BITWISEXOR
    ;

expr_compare:
    EQ
    | NE
    | LE
    | GE
    | LT
    | GT
    ;

expr_modifier:
    PLUS
    | MINUS
    | NOT
    ;

expr_arguments:
    COMMA expr expr_arguments
    | 
    ;

expr_values:
    ID 
    | INTLIT 
    | CHRLIT 
    | REALLIT 
    | LPAR expr RPAR
    ;

%%

void pprint(char* string){
    //printf("%s\n", string);
}
/*
statement: expression '\n'  

	| statement expression '\n'
	;

calc:
     calc '+' calc   {$$=$1+$3;}
    |   calc '-' calc       {$$=$1-$3;}
    |   calc '*' calc       {$$=$1*$3;}
    |   calc '/' calc       {if($3==0) printf("Divide by zero!\n");
                                        else $$=$1/$3;}
    |   '(' calc ')'              {$$=$2;}
    |   NUMBER                          {$$=$1;}
    |   VAR                              {$$=symlook($1)->value;}
    |   '-' calc %prec FAKEMINUS  {$$=-$2;}
    ;
 
expression: 
	VAR '=' calc {symlook($1)->value = $3; if(DEBUG)printf("Atribui %d a %s\n", $3, $1);}	/*Guarda valor da variavel*/
/*	| VAR	{printf("o valor da variavel %s e' %d\n", symlook($1)->name, symlook($1)->value);} *//*Reproduz o valor*/
/*	;*/

