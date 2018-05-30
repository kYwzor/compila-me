; ModuleID = 'enunciado_2.c'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

; Function Attrs: nounwind
define i32 @readint() #0 {
  %read = alloca i32, align 4
  %sign = alloca i32, align 4
  %c = alloca i8, align 1
  store i32 0, i32* %read, align 4
  store i32 1, i32* %sign, align 4
  %1 = call i32 bitcast (i32 (...)* @getchar to i32 ()*)()
  %2 = trunc i32 %1 to i8
  store i8 %2, i8* %c, align 1
  %3 = load i8, i8* %c, align 1
  %4 = sext i8 %3 to i32
  %5 = icmp eq i32 %4, 45
  br i1 %5, label %6, label %7

; <label>:6                                       ; preds = %0
  store i32 -1, i32* %sign, align 4
  br label %7

; <label>:7                                       ; preds = %6, %0
  br label %8

; <label>:8                                       ; preds = %23, %7
  %9 = load i8, i8* %c, align 1
  %10 = sext i8 %9 to i32
  %11 = icmp ne i32 %10, 10
  br i1 %11, label %12, label %26

; <label>:12                                      ; preds = %8
  %13 = load i8, i8* %c, align 1
  %14 = sext i8 %13 to i32
  %15 = icmp ne i32 %14, 45
  br i1 %15, label %16, label %23

; <label>:16                                      ; preds = %12
  %17 = load i32, i32* %read, align 4
  %18 = mul nsw i32 %17, 10
  %19 = load i8, i8* %c, align 1
  %20 = sext i8 %19 to i32
  %21 = add nsw i32 %18, %20
  %22 = sub nsw i32 %21, 48
  store i32 %22, i32* %read, align 4
  br label %23

; <label>:23                                      ; preds = %16, %12
  %24 = call i32 bitcast (i32 (...)* @getchar to i32 ()*)()
  %25 = trunc i32 %24 to i8
  store i8 %25, i8* %c, align 1
  br label %8

; <label>:26                                      ; preds = %8
  %27 = load i32, i32* %sign, align 4
  %28 = load i32, i32* %read, align 4
  %29 = mul nsw i32 %27, %28
  ret i32 %29
}

declare i32 @getchar(...) #1

; Function Attrs: nounwind
define void @printint(i32 %n) #0 {
  %1 = alloca i32, align 4
  store i32 %n, i32* %1, align 4
  %2 = load i32, i32* %1, align 4
  %3 = icmp slt i32 %2, 0
  br i1 %3, label %4, label %8

; <label>:4                                       ; preds = %0
  %5 = call i32 bitcast (i32 (...)* @putchar to i32 (i32)*)(i32 45)
  %6 = load i32, i32* %1, align 4
  %7 = sub nsw i32 0, %6
  store i32 %7, i32* %1, align 4
  br label %8

; <label>:8                                       ; preds = %4, %0
  %9 = load i32, i32* %1, align 4
  %10 = sdiv i32 %9, 10
  %11 = icmp ne i32 %10, 0
  br i1 %11, label %12, label %15

; <label>:12                                      ; preds = %8
  %13 = load i32, i32* %1, align 4
  %14 = sdiv i32 %13, 10
  call void @printint(i32 %14)
  br label %15

; <label>:15                                      ; preds = %12, %8
  %16 = load i32, i32* %1, align 4
  %17 = srem i32 %16, 10
  %18 = add nsw i32 %17, 48
  %19 = call i32 bitcast (i32 (...)* @putchar to i32 (i32)*)(i32 %18)
  ret void
}

declare i32 @putchar(...) #1

; Function Attrs: nounwind
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 1, i32* %i, align 4
  br label %2

; <label>:2                                       ; preds = %5, %0
  %3 = load i32, i32* %i, align 4
  %4 = icmp ne i32 %3, 0
  br i1 %4, label %5, label %9

; <label>:5                                       ; preds = %2
  %6 = call i32 @readint()
  store i32 %6, i32* %i, align 4
  %7 = load i32, i32* %i, align 4
  call void @printint(i32 %7)
  %8 = call i32 bitcast (i32 (...)* @putchar to i32 (i32)*)(i32 10)
  br label %2

; <label>:9                                       ; preds = %2
  %10 = load i32, i32* %1, align 4
  ret i32 %10
}

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.1-24ubuntu7 (tags/RELEASE_381/final)"}
