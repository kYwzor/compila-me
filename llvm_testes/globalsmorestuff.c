int a = 4;
double b = 2.24;
int c = 2;
short d = 2;
char e = 'a';

void messed_up(int c);

int main(void){
	putchar(a + 'A');
	putchar('\n');
	putchar(b + 'A');
	putchar('\n');
	putchar(c + 'A');
	putchar('\n');
	putchar(d + 'A');
	putchar('\n');
	putchar(e + 'A');
	putchar('\n');

	short a = 1;
	d = a + e;
	putchar(a + 'A');
	putchar('\n');
	putchar(d + 'A');
	putchar('\n');
	putchar(e + 'A');
	putchar('\n');

	messed_up(-2);
	e = 'b';
	putchar(e + 'A');
	putchar('\n');


	return 0;
}

void messed_up(int c){
	putchar(a + 'A');
	putchar('\n');
	putchar(c + 'A');
	putchar('\n');
	putchar(d + 'A');
	putchar('\n');
	putchar(e + 'A');
	putchar('\n');
	return;
}