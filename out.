declare i32 @putchar(i32)
declare i32 @getchar()
define void @unary_minus(){
%a = alloca i32
%1 = add i32 0, 1
store i32 %1, i32* %a
%b = alloca i32
%2 = load i32, i32* %a
%3 = sub nsw i32 0, %2
store i32 %3, i32* %b
%4 = add i32 0, 1
store i32 %4, i32* %a
%5 = add i32 0, 2
store i32 %5, i32* %a
%c = alloca i8
%6 = add i32 0, 97
%7 = trunc i32 %6 to i8
store i8 %7, i8* %c
%d = alloca i8
%8 = load i8, i8* %c
%9 = sub nsw i8 0, %8
store i8 %9, i8* %d
%10 = load i8, i8* %c
%11 = sext i8 %10 to i32
%12 = call i32 @putchar(i32 %11)
ret void 
}
define i8 @exemplo(){
%a = alloca i8
%1 = add i32 0, 97
%2 = trunc i32 %1 to i8
store i8 %2, i8* %a
%3 = load i8, i8* %a
%4 = sext i8 %3 to i32
%5 = call i32 @putchar(i32 %4)
%6 = load i8, i8* %a
ret i8 %6
ret i8 0
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
%5 = trunc i32 %4 to i16
store i16 %5, i16* %d
%e = alloca i16
%6 = add i32 0, 39
store i32 %6, i32* %a
%7 = trunc i32 %6 to i16
store i16 %7, i16* %e
%f = alloca i8
%8 = add i32 0, 37
%9 = trunc i32 %8 to i8
store i8 %9, i8* %f
%g = alloca i8
%10 = add i32 0, 38
store i32 %10, i32* %a
%11 = trunc i32 %10 to i8
store i8 %11, i8* %g
%h = alloca double
%12 = add i32 0, 45
%13 = sitofp i32 %12 to double
store double %13, double* %h
%j = alloca double
%14 = add i32 0, 99
store i32 %14, i32* %a
%15 = sitofp i32 %14 to double
store double %15, double* %j
ret void 
}
define i16 @shawty(){
%a = alloca i16
%1 = add i32 0, 1
%2 = trunc i32 %1 to i16
store i16 %2, i16* %a
%3 = load i16, i16* %a
ret i16 %3
ret i16 0
}
define i8 @charizard(){
%a = alloca i8
%1 = add i32 0, 2
%2 = trunc i32 %1 to i8
store i8 %2, i8* %a
%3 = load i8, i8* %a
ret i8 %3
ret i8 0
}
define i32 @inteligente(){
%a = alloca i32
%1 = add i32 0, 1
store i32 %1, i32* %a
%2 = load i32, i32* %a
ret i32 %2
ret i32 0
}
define i32 @soma(i32 %a, i32 %b){
%1 = alloca i32
store i32 %a, i32* %1
%2 = alloca i32
store i32 %b, i32* %2
%3 = load i32, i32* %a
%4 = load i32, i32* %b
%5 = add i32 %3, %4
ret i32 %5
ret i32 0
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
%b = alloca i8
%3 = add i32 0, 4
%4 = trunc i32 %3 to i8
store i8 %4, i8* %b
%5 = call i8 @charizard()
store i8 %5, i8* %b
%a = alloca i32
%6 = add i32 0, 1
store i32 %6, i32* %a
%7 = call i16 @shawty()
%8 = sext i16 %7 to i32
store i32 %8, i32* %a
%c = alloca i8
%9 = call i32 @inteligente()
%10 = trunc i32 %9 to i8
store i8 %10, i8* %c
%oi = alloca i32
%11 = add i32 0, 1
%12 = add i32 0, 2
%13 = call i32 @soma(i32 %11, i32 %12)
store i32 %13, i32* %oi
%14 = add i32 0, 1
ret i32 %14
ret i32 0
}
