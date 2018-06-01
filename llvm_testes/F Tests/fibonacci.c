int f(int);
 
int main()
{
  int n, i = 0, c;
 
  n = getchar();
  
  c = 1;
  while(c <= n){
	putchar(f(i));
	i = i + 1;
  }
  
  return 0;
}
 
int f(int n)
{
  if (n == 0 || n == 1)
    return n;
  else
    return (f(n-1) + f(n-2));
}