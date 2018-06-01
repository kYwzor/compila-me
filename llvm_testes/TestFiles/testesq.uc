int teste(double a, double c, int d) {
    a = 'b';
    a = 3e1 + (d = 4);
    c = a + c;
    char g = 48;
    return d;
}

int main(void) {
    short f = teste(1, 2e2, 2);
    putchar(f + 48);
}