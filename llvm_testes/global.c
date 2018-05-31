char a = 'a';
void voidling(void){
    putchar(a);
}
void voidparam(char a){
    putchar(a);
}
char charmeleon(char a){
    return a + 2;
}
char charmander(char a){
    a = a + 1;
    a = charmeleon(a);
    return a;
}

char b = 'b';
int main(void){
    putchar(a);
    char a = 'c';
    putchar(a);
    putchar(b);
    voidling();
    voidparam(a);
    a = charmander(a);
    putchar(a);
}
