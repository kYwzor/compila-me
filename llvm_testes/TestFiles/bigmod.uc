int bigmod(int a, int p, int m) {
    int c;
    if(p == 0) {
        return 1;
    }
    if(p % 2) {
        return ((a % m) * (bigmod(a, p - 1, m))) % m;
    }
    else {
        c = bigmod(a, p / 2, m);
        return ((c % m) * (c % m)) % m;
    }
}

int main(void) {
    int a = 3, p = 3, m = 5;
    
    int k = bigmod(a, p, m);
    putchar(k + 48);
    putchar(10);
}