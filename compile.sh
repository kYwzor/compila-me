#!/bin/sh
lex uccompiler.l
yacc -d -v uccompiler.y
clang-3.8 -Wall -o uccompiler y.tab.c lex.yy.c
clang-3.8 -Wall -Wno-unused-function *.c