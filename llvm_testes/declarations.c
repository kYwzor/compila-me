//#include <stdio.h>
void unary_minus(void){
    int a = -1;
    int b = -a;
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
