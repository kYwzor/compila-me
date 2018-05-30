declare i32 @putchar(i32)
declare i32 @getchar()
define void @unary_minus(){
%a = alloca i32
%1 = add i32 0, 1
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
define i32 @main(){
%a = alloca i32
%1 = add i32 0, 97
store i32 %1, i32* %a
%b = alloca i32
%2 = add i32 0, 50
store i32 %2, i32* %a
store i32 %2, i32* %b
%c = alloca i32
%3 = load i32, i32* %a
store i32 %3, i32* %c
call void @unary_minus()
ret i32 0
}
