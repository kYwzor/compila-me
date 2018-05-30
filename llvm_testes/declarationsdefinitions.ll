; ModuleID = 'declarationsdefinitions.c'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

; Function Attrs: nounwind
define double @hmm(i16 signext %test) #0 {
  %1 = alloca i16, align 2
  store i16 %test, i16* %1, align 2
  ret double 2.000000e+00
}

; Function Attrs: nounwind
define signext i16 @a(double %hm) #0 {
  %1 = alloca double, align 8
  store double %hm, double* %1, align 8
  ret i16 1
}

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1-24ubuntu7 (tags/RELEASE_381/final)"}
