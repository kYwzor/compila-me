int f(int);
 
int main(void)
{
  int n, i = 0;
 
  n = getchar();
  
  while(i <= n){
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