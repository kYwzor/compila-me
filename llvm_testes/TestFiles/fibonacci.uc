int f(int);

int f(int n) {
    if(n == 0 || n == 1) {
        return n;
    }
    else {
        return (f(n-1) + f(n-2));
    }
}

int main(void) {
    int n = 7, i = 0, c = 1;

    while(c <= n) {
        putchar(f(i) + 48);
        putchar(10);
        i = i + 1;
        c = c + 1;
    }

    return 1;
}