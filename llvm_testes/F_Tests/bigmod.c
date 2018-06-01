int bigmod(int a, int p, int m);

int main(void)
{
      int a;
      int p,m;

      putchar(bigmod(5,55,231));

      return 0;
}

int bigmod(int a, int p, int m)
{
      int c;
            if(p==0)
            return 1;

            if(p%2==0){                  // p is even then split it up and mod
            c=bigmod(a,p/2,m);
            return ( (c%m) * (c%m) )%m;
            }
            else  // p is odd then make it even
            return ( (a%m)* bigmod(a,p-1,m) ) %m;

}