; ModuleID = 'enunciado_1.c'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

; Function Attrs: nounwind
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %i = alloca i8, align 1
  store i32 0, i32* %1, align 4
  store i8 65, i8* %i, align 1
  br label %2

; <label>:2                                       ; preds = %6, %0
  %3 = load i8, i8* %i, align 1
  %4 = sext i8 %3 to i32
  %5 = icmp sle i32 %4, 90
  br i1 %5, label %6, label %14

; <label>:6                                       ; preds = %2
  %7 = load i8, i8* %i, align 1
  %8 = sext i8 %7 to i32
  %9 = call i32 bitcast (i32 (...)* @putchar to i32 (i32)*)(i32 %8)
  %10 = load i8, i8* %i, align 1
  %11 = sext i8 %10 to i32
  %12 = add nsw i32 %11, 1
  %13 = trunc i32 %12 to i8
  store i8 %13, i8* %i, align 1
  br label %2

; <label>:14                                      ; preds = %2
  ret i32 0
}

declare i32 @putchar(...) #1

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1-24ubuntu7 (tags/RELEASE_381/final)"}
