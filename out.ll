declare i32 @putchar(i32)
declare i32 @getchar()
define i32 @main(){
%a = alloca i32
%1 = add i32 0, 1
store i32 %1, i32* %a
%b = alloca i32
%2 = add i32 0, 2
store i32 %2, i32* %b
%c = alloca i32
%3 = load i32, i32* %a
%4 = load i32, i32* %b
%5 = add i32 %3, %4
store i32 %5, i32* %c
%d = alloca i32
%6 = add i32 0, 3
%7 = add i32 0, 4
%8 = add i32 %6, %7
store i32 %8, i32* %d
%9 = add i32 0, 97
%10 = call i32 @putchar(i32 %9)
%11 = load i32, i32* %c
ret i32 %11
ret i32 0
}
