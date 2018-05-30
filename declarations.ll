; ModuleID = 'llvm_testes/declarations.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @unary_minus() #0 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i8, align 1
  %d = alloca i8, align 1
  store i32 1, i32* %a, align 4
  %1 = load i32, i32* %a, align 4
  %2 = sub nsw i32 0, %1
  store i32 %2, i32* %b, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %a, align 4
  store i8 97, i8* %c, align 1
  %3 = load i8, i8* %c, align 1
  %4 = sext i8 %3 to i32
  %5 = sub nsw i32 0, %4
  %6 = trunc i32 %5 to i8
  store i8 %6, i8* %d, align 1
  %7 = load i8, i8* %c, align 1
  %8 = sext i8 %7 to i32
  %9 = call i32 @putchar(i32 %8)
  ret void
}

declare i32 @putchar(i32) #1

; Function Attrs: nounwind uwtable
define signext i8 @exemplo() #0 {
  %a = alloca i8, align 1
  store i8 97, i8* %a, align 1
  %1 = load i8, i8* %a, align 1
  %2 = sext i8 %1 to i32
  %3 = call i32 @putchar(i32 %2)
  %4 = load i8, i8* %a, align 1
  ret i8 %4
}

; Function Attrs: nounwind uwtable
define void @encadeados() #0 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i16, align 2
  %e = alloca i16, align 2
  %f = alloca i8, align 1
  %g = alloca i8, align 1
  %h = alloca double, align 8
  %j = alloca double, align 8
  store i32 37, i32* %a, align 4
  store i32 38, i32* %a, align 4
  store i32 38, i32* %b, align 4
  %1 = load i32, i32* %a, align 4
  store i32 %1, i32* %c, align 4
  store i16 37, i16* %d, align 2
  store i32 39, i32* %a, align 4
  store i16 39, i16* %e, align 2
  store i8 37, i8* %f, align 1
  store i32 38, i32* %a, align 4
  store i8 38, i8* %g, align 1
  store double 4.500000e+01, double* %h, align 8
  store i32 99, i32* %a, align 4
  store double 9.900000e+01, double* %j, align 8
  ret void
}

; Function Attrs: nounwind uwtable
define signext i16 @shawty() #0 {
  %a = alloca i16, align 2
  store i16 1, i16* %a, align 2
  %1 = load i16, i16* %a, align 2
  ret i16 %1
}

; Function Attrs: nounwind uwtable
define signext i8 @charizard() #0 {
  %a = alloca i8, align 1
  store i8 2, i8* %a, align 1
  %1 = load i8, i8* %a, align 1
  ret i8 %1
}

; Function Attrs: nounwind uwtable
define i32 @inteligente() #0 {
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %1 = load i32, i32* %a, align 4
  ret i32 %1
}

; Function Attrs: nounwind uwtable
define i32 @soma(i32 %a, i32 %b) #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 %a, i32* %1, align 4
  store i32 %b, i32* %2, align 4
  %3 = load i32, i32* %1, align 4
  %4 = load i32, i32* %2, align 4
  %5 = add nsw i32 %3, %4
  ret i32 %5
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %d = alloca double, align 8
  %e = alloca double, align 8
  %b = alloca i8, align 1
  %a = alloca i32, align 4
  %c = alloca i8, align 1
  %oi = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store double 1.000000e+00, double* %d, align 8
  %2 = load double, double* %d, align 8
  store double %2, double* %e, align 8
  call void @encadeados()
  call void @unary_minus()
  store i8 4, i8* %b, align 1
  %3 = call signext i8 @charizard()
  store i8 %3, i8* %b, align 1
  store i32 1, i32* %a, align 4
  %4 = call signext i16 @shawty()
  %5 = sext i16 %4 to i32
  store i32 %5, i32* %a, align 4
  %6 = call i32 @inteligente()
  %7 = trunc i32 %6 to i8
  store i8 %7, i8* %c, align 1
  %8 = call i32 @soma(i32 1, i32 2)
  store i32 %8, i32* %oi, align 4
  ret i32 1
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0-2ubuntu3~trusty5 (tags/RELEASE_380/final)"}
