; ModuleID = 'llvm_testes/returns.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @mainInt() #0 {
  ret i32 1
}

; Function Attrs: nounwind uwtable
define void @mainVoid() #0 {
  ret void
}

; Function Attrs: nounwind uwtable
define double @mainDouble() #0 {
  ret double 1.100000e+00
}

; Function Attrs: nounwind uwtable
define double @mainDoubleExp() #0 {
  ret double 1.100000e+00
}

; Function Attrs: nounwind uwtable
define double @mainDoubleSigned() #0 {
  ret double -1.100000e+00
}

; Function Attrs: nounwind uwtable
define signext i8 @mainChar() #0 {
  ret i8 97
}

; Function Attrs: nounwind uwtable
define signext i8 @mainCharTricky() #0 {
  ret i8 9
}

; Function Attrs: nounwind uwtable
define signext i16 @mainShort() #0 {
  ret i16 1
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0-2ubuntu3~trusty5 (tags/RELEASE_380/final)"}
