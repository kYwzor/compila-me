int fatorial(int n);
int main(void){
    putchar(fatorial(5) + 'A');
	putchar('\n');
    putchar(fatorial(13) + 'A');
	putchar('\n');
    putchar(fatorial(5) + 'A');
	putchar('\n');
    putchar(fatorial(2) + 'A');
	putchar('\n');
    putchar(fatorial(10) + 'A');
	putchar('\n');
}
int fatorial(int n){
    if (n == 1){
        return 1;
    } 
    return n * fatorial(n - 1);
}