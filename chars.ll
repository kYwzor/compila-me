; ModuleID = 'llvm_testes/chars.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %a = alloca i8, align 1
  store i8 39, i8* %a, align 1
  %1 = load i8, i8* %a, align 1
  %2 = sext i8 %1 to i32
  %3 = call i32 @putchar(i32 %2)
  store i8 10, i8* %a, align 1
  %4 = load i8, i8* %a, align 1
  %5 = sext i8 %4 to i32
  %6 = call i32 @putchar(i32 %5)
  store i8 34, i8* %a, align 1
  %7 = load i8, i8* %a, align 1
  %8 = sext i8 %7 to i32
  %9 = call i32 @putchar(i32 %8)
  store i8 9, i8* %a, align 1
  %10 = load i8, i8* %a, align 1
  %11 = sext i8 %10 to i32
  %12 = call i32 @putchar(i32 %11)
  store i8 92, i8* %a, align 1
  %13 = load i8, i8* %a, align 1
  %14 = sext i8 %13 to i32
  %15 = call i32 @putchar(i32 %14)
  ret i32 0
}

declare i32 @putchar(i32) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0-2ubuntu3~trusty5 (tags/RELEASE_380/final)"}
