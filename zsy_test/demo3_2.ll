; ModuleID = 'demo3_1.ll'
source_filename = "demo3.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @zsy_test(i32 noundef %0) #0 {
  br label %2

2:                                                ; preds = %27, %1
  %.0 = phi i32 [ 0, %1 ], [ %28, %27 ]
  %3 = icmp slt i32 %.0, 10
  br i1 %3, label %4, label %29

4:                                                ; preds = %2
  br label %5

5:                                                ; preds = %4
  %6 = add nsw i32 0, 2
  %7 = icmp slt i32 %0, 10
  br i1 %7, label %8, label %9

8:                                                ; preds = %5
  br label %10

9:                                                ; preds = %5
  br label %15

10:                                               ; preds = %8
  %11 = add nsw i32 %6, 3
  %12 = icmp slt i32 %0, 20
  br i1 %12, label %13, label %14

13:                                               ; preds = %10
  br label %19

14:                                               ; preds = %10
  br label %21

15:                                               ; preds = %9
  %16 = icmp slt i32 %0, 30
  br i1 %16, label %17, label %18

17:                                               ; preds = %15
  br label %21

18:                                               ; preds = %15
  br label %25

19:                                               ; preds = %13
  %20 = add nsw i32 %11, 4
  br label %23

21:                                               ; preds = %17, %14
  %.01 = phi i32 [ %11, %14 ], [ %6, %17 ]
  %22 = add nsw i32 %.01, 5
  br label %23

23:                                               ; preds = %21, %19
  %.1 = phi i32 [ %20, %19 ], [ %22, %21 ]
  %24 = add nsw i32 %.1, 7
  br label %25

25:                                               ; preds = %23, %18
  %.2 = phi i32 [ %24, %23 ], [ %6, %18 ]
  %26 = add nsw i32 %.2, 8
  br label %27

27:                                               ; preds = %25
  %28 = add nsw i32 %.0, 1
  br label %2, !llvm.loop !4

29:                                               ; preds = %2
  ret void
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
