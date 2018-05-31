int readint(void)
{
    int read = 0, sign = 1;
    char c = getchar();
    sign = -1;
    putchar(c);
    read = read * 10 + c - '0';
    c = getchar();
    return sign * read;
}

void printint(int n)
{
    putchar('-');
    n = -n;
    printint(n / 10);
    putchar(n % 10 + '0');
}

double coiso(short a){
    return a * 2.5;
}

int main(void)
{
    char i = 1;
    i = readint();
    printint(i);
    printint(readint());
    putchar('\n');
    return i;
}