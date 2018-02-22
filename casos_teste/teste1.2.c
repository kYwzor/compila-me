/*a
*/
''
int main(void)
{
    char i = '\'';
i='ab;aa
    i = ' ';
    i = '\\';
    i = '\t';
    i = '\n';
    i = '\"';
    i = '\'';
    i = '\000';
    //
    /* ola
    salut*/

    /* sdfsdfs
    dfsdfsdf*/ char pila;
    /*Unterminated*/
    i = 'ab;aa
    i = 'a
    i = '\4\';
    i = '\';
    i = '\000' ab;
    i = 'a';
    /*Invalid*/
    i = 'a'a;k
    i = '\`';
    i = '\090';
    i = 'ab'cd';
    i = 'ab\'cd';
    i = '\ ';
    i = '';
    i = '''; //Also Unterminated
    while (i <= 'Z')
    {
        putchar(i);
        i = i + 1;
    }
    return 0;
}