int fibonacci(int n){
	if(n==0){
		return 1;
	}
	if(n == 1){
		return 1;
	}
	return fibonacci(n-1) + fibonacci(n-2);
}


int main(void){
	int i = 0;
	while(i < 10){
		putchar(fibonacci(i) + 'A');
		putchar('\n');
		i = i+1;
	}
}


