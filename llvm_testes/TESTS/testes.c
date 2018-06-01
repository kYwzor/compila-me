int factorial(int n){
	if(n <= 1) return 1;
	return n * factorial(n-1);
}

int factorial2(int n){
	int res = 1;
	while(n > 1){
		res = res * n;
		n = n-1;
	}
	return res;
}

int main(void) {
	putchar(factorial(5));
	putchar(factorial2(5));
}

