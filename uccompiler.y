%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #define DEBUG 1
    int yylex(void);
    void pprint(char* string);
    void yyerror (const char *s);
%}

%token REALLIT 
%token INTLIT
%token CHRLIT
%token CHAR
%token ELSE
%token WHILE
%token IF
%token INT
%token SHORT
%token DOUBLE
%token RETURN
%token VOID
%token BITWISEAND
%token BITWISEOR
%token BITWISEXOR
%token AND
%token ASSIGN
%token MUL
%token COMMA
%token DIV
%token EQ
%token GE
%token GT
%token LBRACE
%token LE
%token LPAR
%token LT
%token MINUS
%token MOD
%token NE
%token NOT
%token OR 
%token PLUS
%token RBRACE
%token RPAR
%token SEMI
%token RESERVED
%token ID
%left LAST


%union{
    char* value;
}

%%
main:
    typespec
    | expr
    ;
typespec: 
    INT {pprint("Int");}
    | CHAR {pprint("Char");}
    | VOID {pprint("Void");}
    | SHORT {pprint("Short");}
    | DOUBLE {pprint("Double");}
    ;

expr:
    expr expr_assignments expr {pprint("expr_assignments");}
    | expr expr_math expr {pprint("expr_math");}
    | expr expr_logic expr {pprint("expr_logic");}
    | expr expr_compare expr {pprint("expr_compare");}
    | expr_modifier expr %prec LAST {pprint("expr_modifier");}
    | ID LPAR RPAR {pprint("expr_arguments");}
    | ID LPAR expr expr_arguments RPAR {pprint("expr_arguments");}
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
    printf("%s\n", string);
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

