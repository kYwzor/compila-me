//#include <stdio.h>
void unary_minus(void){
    int a = 1;
    int b = -a;
    a = 1;
    a = 2;
    char c = 'a';
    char d = -c;
    putchar(c);
}

char exemplo(void){
    char a = 'a';
    putchar(a);
    return a;
}

void encadeados(void){
    int a = 37;
    int b = a = 38;
    int c = a;
    short d = 37;
    short e = a = 39;
    char f = 37;
    char g = a = 38;
    double h = 45;
    double j = a = 99;
    //double i = h + g + e + c;
    //int j = g + e + c;
}
short shawty(void){
    short a = 1;
    return a;
}

char charizard(void){
    char a = 2;
    return a;
}
int inteligente(void){
    int a = 1;
    return a;
}

int soma(int a,int b){
    return a + b;
}

int main(void){
    double d  = 1.0;
    double e = d;
    encadeados();
    unary_minus();
    char b = 4;
    b = charizard();
    int a = 1;
    a = shawty();
    char c = inteligente();
    int oi;
    oi = soma(1,2);
    return 1;
}



