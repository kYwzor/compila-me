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
	putchar(gcd(54,24));
	putchar(gcd(88,4));
	putchar(gcd(100,10));
	putchar(gcd(76,6));
	putchar(gcd(321,123));
	return 0;
}