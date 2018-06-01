int gcd(int a, int b)
{
    int temp;
    while (b != 0)
    {
        temp = a % b;

        a = b;
        b = temp;
    }
    return a;
}

int main(void){
	putchar(gcd(54,24) + 'A');
    putchar('\n');
	putchar(gcd(88,4) + 'A');
    putchar('\n');
	putchar(gcd(100,10) + 'A');
    putchar('\n');
	putchar(gcd(76,6) + 'A');
    putchar('\n');
	putchar(gcd(321,123) + 'A');
    putchar('\n');
	return 0;
}