declare i32 @putchar(i32)
declare i32 @getchar()
define void @unary_minus(){
%a = alloca i32
FATAL: Invalid llvm type Empty
%1 = add (null) 0, 1
%2 = sub nsw i32 0, %1
store i32 %2, i32* %a
%b = alloca i32
%3 = load i32, i32* %a
%4 = sub nsw i32 0, %3
store i32 %4, i32* %b
%5 = add i32 0, 1
store i32 %5, i32* %a
ret void 
}
define void @encadeados(){
%a = alloca i32
%1 = add i32 0, 37
store i32 %1, i32* %a
%b = alloca i32
%2 = add i32 0, 38
store i32 %2, i32* %a
store i32 %2, i32* %b
%c = alloca i32
%3 = load i32, i32* %a
store i32 %3, i32* %c
%d = alloca i16
%4 = add i32 0, 37
store i16 %4, i16* %d
%e = alloca i16
%5 = add i32 0, 39
store i32 %5, i32* %a
store i16 %5, i16* %e
%f = alloca i8
%6 = add i32 0, 37
store i8 %6, i8* %f
%g = alloca i8
%7 = add i32 0, 38
store i32 %7, i32* %a
store i8 %7, i8* %g
%h = alloca double
%8 = add i32 0, 45
store double %8, double* %h
%j = alloca double
%9 = add i32 0, 99
store i32 %9, i32* %a
store double %9, double* %j
ret void 
}
define i32 @main(){
%d = alloca double
%1 = fadd double 0.000000e+00, 1.0000000000000000E+00
store double %1, double* %d
%e = alloca double
%2 = load double, double* %d
store double %2, double* %e
call void @encadeados()
call void @unary_minus()
ret i32 0
}
