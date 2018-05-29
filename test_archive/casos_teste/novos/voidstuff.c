int a;
void a;

int sa(void, void, void);

int sa(int a, char b){
	return 1;
}

int sa(int, char);

int b(int, int);

int b(int, int, void){
	return 0;
}

int c(int, int);
int c(int a, int b, void){
	return 2;
}

int d(int, int);
int d(int, int){
	return -1;
}

int e(int a, int b){
	return -2;
}

int e(int a, void);

int f(int a, int b, int c);

int f(int a, char b, void);