declare i32 @putchar(i32)
declare i32 @getchar()
define i32 @main(){
%a = alloca i32
%1 = add i32 0, 39
store i32 %1, i32* %a
%2 = load i32, i32* %a
%3 = call i32 @putchar(i32 %2)
%4 = add i32 0, 10
store i32 %4, i32* %a
%5 = load i32, i32* %a
%6 = call i32 @putchar(i32 %5)
%7 = add i32 0, 34
store i32 %7, i32* %a
%8 = load i32, i32* %a
%9 = call i32 @putchar(i32 %8)
%10 = add i32 0, 9
store i32 %10, i32* %a
%11 = load i32, i32* %a
%12 = call i32 @putchar(i32 %11)
%13 = add i32 0, 92
store i32 %13, i32* %a
%14 = load i32, i32* %a
%15 = call i32 @putchar(i32 %14)
ret i32 0
}
