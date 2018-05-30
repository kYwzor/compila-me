//#include <stdio.h>
void unary_minus(void){
    int a = -1;
    int b = -a;
    a = 1;
    a = 2;
    char c = 'a';
    char d = -c;
    putchar(c);
}
int main(void){
    int a = 'a';
    int b = a = '2';
    int c = a;
    unary_minus();
}

