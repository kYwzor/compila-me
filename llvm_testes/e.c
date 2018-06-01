int soma(int a, int b);
void func(int a);
void testa_rel(void);

int main(void){
	testa_rel();
	/*
	int a=0;
	putchar(a+48);
	func(a);
	putchar(a+48);
	int f = 20==(20+1);
	putchar(f+48);    
    putchar(soma(soma('a','a'),soma(2, 5)));

	*/

}

int soma(int a, int b){
    char c = a;
    char d = b;
    return 'a';
}
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
