cmake_minimum_required(VERSION 3.9)
project(compila_me)

set(CMAKE_CXX_STANDARD 11)

add_executable(compila_me
        lex.yy.c
        y.tab.c
        y.tab.h)
