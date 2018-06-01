int main(void){
	int a = 1;
	int b = 0;
	putchar(a || a);
	putchar(a || b);
	putchar(b || a);
	putchar(b || b);

	putchar(a && a);
	putchar(a && b);
	putchar(b && a);
	putchar(b && b);

	int c;
	c = a || a || a;
	c = a || a || b;
	c = a || b || a;
	c = a || b || b;
	c = b || a || a;
	c = b || a || b;
	c = b || b || a;
	c = b || b || b;


	c = a && a && a;
	c = a && a && b;
	c = a && b && a;
	c = a && b && b;
	c = b && a && a;
	c = b && a && b;
	c = b && b && a;
	c = b && b && b;
	return 0;
}