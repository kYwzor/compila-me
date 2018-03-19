%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #define DEBUG 1
    int yylex(void);
    void yyerror (const char *s);
%}

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

%union{
    char* value;
}

%%

statement: COMMA {printf("COMMA\n");}


%%
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

