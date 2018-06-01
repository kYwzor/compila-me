int fatorial(int n);
int main(void){
    putchar(fatorial(5));
}
int fatorial(int n){
    if (n == 1){
        return 1;
    } 
    return n * fatorial(n - 1);
}