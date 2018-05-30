; ModuleID = 'llvm_testes/declarations.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @unary_minus() #0 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i8, align 1
  %d = alloca i8, align 1
  store i32 -1, i32* %a, align 4
  %1 = load i32, i32* %a, align 4
  %2 = sub nsw i32 0, %1
  store i32 %2, i32* %b, align 4
  store i8 97, i8* %c, align 1
  %3 = load i8, i8* %c, align 1
  %4 = sext i8 %3 to i32
  %5 = sub nsw i32 0, %4
  %6 = trunc i32 %5 to i8
  store i8 %6, i8* %d, align 1
  %7 = load i8, i8* %d, align 1
  %8 = sext i8 %7 to i32
  %9 = call i32 @putchar(i32 %8)
  ret void
}

declare i32 @putchar(i32) #1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 97, i32* %a, align 4
  store i32 50, i32* %a, align 4
  store i32 50, i32* %b, align 4
  %1 = load i32, i32* %a, align 4
  store i32 %1, i32* %c, align 4
  call void @unary_minus()
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0-2ubuntu3~trusty5 (tags/RELEASE_380/final)"}
