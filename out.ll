declare i32 @putchar(i32)
declare i32 @getchar()
define i32 @main(){
%a = alloca i8
%1 = add i32 0, 0
store i8 %1, i8* %a
%2 = load i8, i8* %a
%3 = call i32 @putchar(i32 %2)
%4 = add i32 0, 0
store i8 %4, i8* %a
%5 = load i8, i8* %a
%6 = call i32 @putchar(i32 %5)
%7 = add i32 0, 0
store i8 %7, i8* %a
%8 = load i8, i8* %a
%9 = call i32 @putchar(i32 %8)
%10 = add i32 0, 0
store i8 %10, i8* %a
%11 = load i8, i8* %a
%12 = call i32 @putchar(i32 %11)
%13 = add i32 0, 0
store i8 %13, i8* %a
%14 = load i8, i8* %a
%15 = call i32 @putchar(i32 %14)
ret i32 0
}
