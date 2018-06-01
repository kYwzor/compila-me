int i = 1;
	char c = 'a';
	short s = 2;
	double d = 2.5;
	int res;

int main(void){
	
	res = i < c;
	res = c > s;
	res = c <= s;
	res = c >= s;
	res = d < c;
	res = d > c;
	res = d == c;
	res = d != c;
	res = d >= c;
	res = d <= c;
	d = -d;
	d = +d;
	res = c == s;
	res = c != s;
	res = s + s;
	res = c * s;
	res = s / i;
	res = c - i;
	d = d * d;
	d = d + s;
	d = d * s;
	d = d / s;
	d = s / d;
	d = d - c;
	res = c % s;
	res = i % i;
	res = !c;
	putchar('\'');
}


