; ModuleID = 'demo3_2.ll'
source_filename = "demo3.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @zsy_test(i32 noundef %0) #0 {
  br label %2

2:
  %.0 = phi i32 [ 0, %1 ], [ %28, %27 ]
  %3 = icmp slt i32 %.0, 10
  br i1 %3, label %S1, label %29

S1:
  %4 = add nsw i32 0, 0
  %5 = add nsw i32 0, 0
  %6 = add nsw i32 0, 2
  %7 = icmp slt i32 %0, 10
  br i1 %7, label %S2, label %S3

S2:
  %8 = add nsw i32 0, 0
  %9 = add nsw i32 0, 0
  %10 = add nsw i32 0, 0
  %11 = add nsw i32 %6, 3
  %12 = icmp slt i32 %0, 20
  br i1 %12, label %S4, label %S5

S3:
  %13 = add nsw i32 0, 0
  %14 = add nsw i32 0, 0
  %15 = add nsw i32 0, 0
  %16 = icmp slt i32 %0, 30
  br i1 %16, label %S5, label %S7

S4:
  %17 = add nsw i32 0, 0
  %18 = add nsw i32 0, 0
  %19 = add nsw i32 0, 0
  %20 = add nsw i32 %11, 4
  %21 = add nsw i32 0, 0
  br label %S6

S5:
  %.01 = phi i32 [ %11, %S2 ], [ %6, %S3 ]
  %22 = add nsw i32 %.01, 5
  %23 = add nsw i32 0, 0
  br label %S6

S6:
  %.1 = phi i32 [ %20, %S4 ], [ %22, %S5 ]
  %24 = add nsw i32 %.1, 7
  %25 = add nsw i32 0, 0
  br label %S7

S7:
  %.2 = phi i32 [ %24, %S6 ], [ %6, %S3 ]
  %26 = add nsw i32 %.2, 8
  br label %27

27:
  %28 = add nsw i32 %.0, 1
  br label %2, !llvm.loop !4

29:
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
