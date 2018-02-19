int main(void)
{
    char i = '\'';
    i = ' ';
    i = '\\';
    i = '\t';
    i = '\n';
    i = '\"';
    i = '\`';
    i = '\000';
    i = '\ ';
    i = '\';
    i = '';
    i = '\090';
    i = ''';
    while (i <= 'Z')
    {
        putchar(i);
        i = i + 1;
    }
    return 0;
}