double i(void){}
void k(void);
int j(void){
    int a;
    double b;
    short c;
    char d;
    i() + a;
    i() + b;
    i() + c;
    i() + d;
    i() + k;
    i() + undef;
    !i();
    //not pode ser feito em qualquer coisa que nao seja um double
    +i();
    -i();
    !k;
    +k;
    -k;
    
    i() | a;
    i() | b;
    i() | c;
    i() | d;
    i() | k;
    k | a;
    k | b;
    k | c;
    k | d;
    k | k;
    _ | a;
    _ | b;
    _ | c;
    _ | d;
    _ | k;
    i() || a;
    i() || b;
    i() || c;
    i() || d;
    i() || k;
    k || a;
    k || b;
    k || c;
    k || d;
    k || k;
    _ || a;
    _ || b;
    _ || c;
    _ || d;
    _ || k;
}