int main(void)
{
    char i = '\'';
    i = ' ';
    i = '\\';
    i = '\ ';
    i = '\';
    i = '';
    i = ''';
    while (i <= 'Z')
    {
        putchar(i);
        i = i + 1;
    }
    return 0;
}