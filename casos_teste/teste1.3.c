int main(void)
{
    char i[10];
    i[0] = 1;
    int a = .5e2;
    while (i[0] <= 'Z')
    {
        putchar(i);
        i[0] = i + 1;
    }
    return 0;
}