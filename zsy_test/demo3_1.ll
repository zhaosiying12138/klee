; ModuleID = 'demo3.c'
source_filename = "demo3.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @zsy_test(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  store i32 0, i32* %4, align 4
  br label %5

5:                                                ; preds = %40, %1
  %6 = load i32, i32* %4, align 4
  %7 = icmp slt i32 %6, 10
  br i1 %7, label %8, label %43

8:                                                ; preds = %5
  br label %9

9:                                                ; preds = %8
  store i32 0, i32* %3, align 4
  %10 = load i32, i32* %3, align 4
  %11 = add nsw i32 %10, 2
  store i32 %11, i32* %3, align 4
  %12 = load i32, i32* %2, align 4
  %13 = icmp slt i32 %12, 10
  br i1 %13, label %14, label %15

14:                                               ; preds = %9
  br label %16

15:                                               ; preds = %9
  br label %23

16:                                               ; preds = %14
  %17 = load i32, i32* %3, align 4
  %18 = add nsw i32 %17, 3
  store i32 %18, i32* %3, align 4
  %19 = load i32, i32* %2, align 4
  %20 = icmp slt i32 %19, 20
  br i1 %20, label %21, label %22

21:                                               ; preds = %16
  br label %28

22:                                               ; preds = %16
  br label %31

23:                                               ; preds = %15
  %24 = load i32, i32* %2, align 4
  %25 = icmp slt i32 %24, 30
  br i1 %25, label %26, label %27

26:                                               ; preds = %23
  br label %31

27:                                               ; preds = %23
  br label %37

28:                                               ; preds = %21
  %29 = load i32, i32* %3, align 4
  %30 = add nsw i32 %29, 4
  store i32 %30, i32* %3, align 4
  br label %34

31:                                               ; preds = %26, %22
  %32 = load i32, i32* %3, align 4
  %33 = add nsw i32 %32, 5
  store i32 %33, i32* %3, align 4
  br label %34

34:                                               ; preds = %31, %28
  %35 = load i32, i32* %3, align 4
  %36 = add nsw i32 %35, 7
  store i32 %36, i32* %3, align 4
  br label %37

37:                                               ; preds = %34, %27
  %38 = load i32, i32* %3, align 4
  %39 = add nsw i32 %38, 8
  store i32 %39, i32* %3, align 4
  br label %40

40:                                               ; preds = %37
  %41 = load i32, i32* %4, align 4
  %42 = add nsw i32 %41, 1
  store i32 %42, i32* %4, align 4
  br label %5, !llvm.loop !4

43:                                               ; preds = %5
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %3 = load i32, i32* %2, align 4
  call void @zsy_test(i32 noundef %3)
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 14.0.6 (https://github.com/llvm/llvm-project.git f28c006a5895fc0e329fe15fead81e37457cb1d1)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
