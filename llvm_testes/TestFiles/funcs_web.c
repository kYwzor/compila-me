void func(int a){

	a=2;
	return;
}

void testa_rel(void){
	int a,b,c;
	a=1;
	b=2;
	c=1;
	if(a<b<c){
		putchar('s');
	}
	else{
		putchar('n');	
	}


	a=10;
	if(a<b<c){
		putchar('s');
	}
	else{
		putchar('n');
	}

	a=10;
	b=10;

	if(a<b == b<c){
		putchar('s');
	}
	else{
		putchar('n');
	}
}

int main(void){
	
	int a=0;
	putchar(a+48);
	func(a);
	putchar(a+48);
	putchar('\n');
	testa_rel();


	int f = 20==(20+1);
	putchar(f+48);



	short b=0;
	double m= 'a' & 'a';
	double n= b & b;
	putchar(((((n +1.0)- m) < 0.00000000001) || ((m - (n +1.0)) < 0.00000000001))+48);

	int i= 'a' & 'a';
	int h= b & b;
	putchar((i==(h +1))+48);

	putchar('\n');
	int o1,o2;
	o1=o2=1;
	putchar(o1+48);
	putchar(o2+48);
	putchar('\n');
	putchar('%');
	int beta= o1&o2;
	putchar(beta+48);


	putchar('\n');
	short o3,o4;
	o3=o4=1;
	putchar(o3+48);
	putchar(o4+48);
	putchar('\n');
	int beta1= o3&o4;
	putchar(beta1+48);

	putchar('\n');
	char o5,o6;
	o5=o6=1;
	putchar(o5+48);
	putchar(o6+48);
	putchar('\n');
	int beta2= o5&o6;
	putchar(beta2+48);
	putchar('\n');

	char o7,o8;
	o7=o8='a';
	putchar(o7);
	putchar(o8);
	putchar('\n');
	int beta3= o7&o8;
	putchar(beta3);
	putchar('\n');

	return 0;
}