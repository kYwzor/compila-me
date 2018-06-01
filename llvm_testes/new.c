int a = 1;
int b = 2; 
int c = 3;
int d = 4;

void cenas(int a, int b){
	c = a +b;
	putchar(60 + c);

}


void cenas1(int c){
	putchar(c);
	return putchar(c + d), cenas(c,d);
}


int main(void){
	cenas1(2);
	return 1;
}
