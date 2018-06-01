int fatorial(int n);
int main(void){
    putchar(fatorial(5));
    putchar(fatorial(13));
    putchar(fatorial(5));
    putchar(fatorial(2));
    putchar(fatorial(10));
}
int fatorial(int n){
    if (n == 1){
        return 1;
    } 
    return n * fatorial(n - 1);
}