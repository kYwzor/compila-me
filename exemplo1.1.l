numero				[0-9]+
decimal [\.\,]
%%

\-?{numero}u {printf(" Unsigned ");}
\-?{numero}ul {printf(" Unsigned Long ");}
\-?{numero}                        {printf(" Integer ");}
\-?{numero}f                        {printf(" Float ");}
\-?{numero}({decimal}{numero})?f? {printf(" Float ");}
\-?{numero}l                        {printf(" Long ");}
\-?{numero}({decimal}{numero})?e[\-|\+]{numero} {printf(" Floating Point ");} 


abc {printf(" Token 1!");}
ab+c {printf(" Token 2!");}
a?(bc)+ {printf(" Token 3!");}





%%
int main()
{
yylex();
return 0;
}

int yywrap()
{
return 1;
}

