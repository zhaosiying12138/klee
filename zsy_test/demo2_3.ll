; ModuleID = 'demo2_2.ll'
source_filename = "demo2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@t = dso_local global i32 3, align 4
@exec_var = dso_local global [8 x [8 x i32]] zeroinitializer, align 16
@.str = private unnamed_addr constant [5 x i8] c"%d, \00", align 1
@.str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@__const.main.a = private unnamed_addr constant [8 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8], align 16
@__const.main.b = private unnamed_addr constant [8 x i32] [i32 2, i32 4, i32 6, i32 8, i32 10, i32 11, i32 13, i32 16], align 16
@__const.main.c = private unnamed_addr constant [8 x i32] [i32 1, i32 3, i32 5, i32 7, i32 9, i32 11, i32 13, i32 15], align 16
@.str.2 = private unnamed_addr constant [6 x i8] c"c[]:\09\00", align 1
@.str.3 = private unnamed_addr constant [10 x i8] c"ev[S%d]:\09\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @EXEC(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = sub nsw i32 %1, 1
  %5 = sext i32 %4 to i64
  %6 = getelementptr inbounds [8 x [8 x i32]], [8 x [8 x i32]]* @exec_var, i64 0, i64 %5
  %7 = sext i32 %2 to i64
  %8 = getelementptr inbounds [8 x i32], [8 x i32]* %6, i64 0, i64 %7
  store i32 1, i32* %8, align 4
  ret i32 %0
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @zsy_test(i32* noundef %0, i32* noundef %1, i32* noundef %2) #0 {
  br label %4

4:
  %.0 = phi i32 [ 0, %3 ], [ %77, %76 ]
  %5 = icmp slt i32 %.0, 8
  %6 = add nsw i32 0, 0
  %7 = add nsw i32 0, 0
  br i1 %5, label %S1, label %78

S1:
  %8 = sext i32 %.0 to i64
  %9 = getelementptr inbounds i32, i32* %0, i64 %8
  %10 = load i32, i32* %9, align 4
  %11 = icmp slt i32 %10, 5
  %12 = add nsw i32 0, 0
  %13 = add nsw i32 0, 0
  br i1 %11, label %S2, label %S3

S2:
  %14 = sext i32 %.0 to i64
  %15 = getelementptr inbounds i32, i32* %1, i64 %14
  %16 = load i32, i32* %15, align 4
  %17 = sext i32 %.0 to i64
  %18 = getelementptr inbounds i32, i32* %0, i64 %17
  %19 = load i32, i32* %18, align 4
  %20 = sdiv i32 %16, %19
  %21 = icmp sgt i32 %20, 1
  %22 = add nsw i32 0, 0
  %23 = add nsw i32 0, 0
  %24 = add nsw i32 0, 0
  %25 = add nsw i32 0, 0
  br i1 %21, label %S4, label %S3

S3:
  %26 = sext i32 %.0 to i64
  %27 = getelementptr inbounds i32, i32* %1, i64 %26
  %28 = load i32, i32* %27, align 4
  %29 = sext i32 %.0 to i64
  %30 = getelementptr inbounds i32, i32* %0, i64 %29
  store i32 %28, i32* %30, align 4
  %31 = add nsw i32 0, 0
  br label %S8

S4:
  %32 = sext i32 %.0 to i64
  %33 = getelementptr inbounds i32, i32* %0, i64 %32
  %34 = load i32, i32* %33, align 4
  %35 = load i32, i32* @t, align 4
  %36 = icmp sgt i32 %34, %35
  %37 = add nsw i32 0, 0
  %38 = add nsw i32 0, 0
  br i1 %36, label %S5, label %S6

S5:
  %39 = sext i32 %.0 to i64
  %40 = getelementptr inbounds i32, i32* %1, i64 %39
  %41 = load i32, i32* %40, align 4
  %42 = sext i32 %.0 to i64
  %43 = getelementptr inbounds i32, i32* %0, i64 %42
  %44 = load i32, i32* %43, align 4
  %45 = sub nsw i32 %41, %44
  %46 = load i32, i32* @t, align 4
  %47 = add nsw i32 %45, %46
  %48 = add nsw i32 0, 0
  %49 = add nsw i32 0, 0
  store i32 %47, i32* @t, align 4
  br label %S8

S6:
  %50 = sext i32 %.0 to i64
  %51 = getelementptr inbounds i32, i32* %0, i64 %50
  %52 = load i32, i32* %51, align 4
  %53 = sext i32 %.0 to i64
  %54 = getelementptr inbounds i32, i32* %1, i64 %53
  %55 = load i32, i32* %54, align 4
  %56 = sub nsw i32 %52, %55
  %57 = load i32, i32* @t, align 4
  %58 = add nsw i32 %56, %57
  %59 = add nsw i32 0, 0
  store i32 %58, i32* @t, align 4
  br label %S7

S7:
  %60 = sext i32 %.0 to i64
  %61 = getelementptr inbounds i32, i32* %0, i64 %60
  %62 = load i32, i32* %61, align 4
  %63 = sext i32 %.0 to i64
  %64 = getelementptr inbounds i32, i32* %1, i64 %63
  %65 = add nsw i32 0, 0
  %66 = add nsw i32 0, 0
  store i32 %62, i32* %64, align 4
  br label %S8

S8:
  %67 = sext i32 %.0 to i64
  %68 = getelementptr inbounds i32, i32* %1, i64 %67
  %69 = load i32, i32* %68, align 4
  %70 = sext i32 %.0 to i64
  %71 = getelementptr inbounds i32, i32* %2, i64 %70
  %72 = load i32, i32* %71, align 4
  %73 = add nsw i32 %69, %72
  %74 = sext i32 %.0 to i64
  %75 = getelementptr inbounds i32, i32* %2, i64 %74
  store i32 %73, i32* %75, align 4
  br label %76

76:
  %77 = add nsw i32 %.0, 1
  br label %4, !llvm.loop !4

78:
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @zsy_test2(i32* noundef %0, i32* noundef %1, i32* noundef %2) #0 {
  br label %4

4:                                                ; preds = %90, %3
  %.0 = phi i32 [ 0, %3 ], [ %91, %90 ]
  %5 = icmp slt i32 %.0, 8
  br i1 %5, label %6, label %92

6:                                                ; preds = %4
  br label %7

7:                                                ; preds = %6
  %8 = sext i32 %.0 to i64
  %9 = getelementptr inbounds i32, i32* %0, i64 %8
  %10 = load i32, i32* %9, align 4
  %11 = icmp slt i32 %10, 5
  %12 = zext i1 %11 to i32
  %13 = call i32 @EXEC(i32 noundef %12, i32 noundef 1, i32 noundef %.0)
  %14 = icmp ne i32 %13, 0
  br i1 %14, label %15, label %30

15:                                               ; preds = %7
  br label %16

16:                                               ; preds = %15
  %17 = sext i32 %.0 to i64
  %18 = getelementptr inbounds i32, i32* %1, i64 %17
  %19 = load i32, i32* %18, align 4
  %20 = sext i32 %.0 to i64
  %21 = getelementptr inbounds i32, i32* %0, i64 %20
  %22 = load i32, i32* %21, align 4
  %23 = sdiv i32 %19, %22
  %24 = icmp sgt i32 %23, 1
  %25 = zext i1 %24 to i32
  %26 = call i32 @EXEC(i32 noundef %25, i32 noundef 2, i32 noundef %.0)
  %27 = icmp ne i32 %26, 0
  br i1 %27, label %28, label %29

28:                                               ; preds = %16
  br label %38

29:                                               ; preds = %16
  br label %30

30:                                               ; preds = %29, %7
  br label %31

31:                                               ; preds = %30
  %32 = sext i32 %.0 to i64
  %33 = getelementptr inbounds i32, i32* %1, i64 %32
  %34 = load i32, i32* %33, align 4
  %35 = sext i32 %.0 to i64
  %36 = getelementptr inbounds i32, i32* %0, i64 %35
  store i32 %34, i32* %36, align 4
  %37 = call i32 @EXEC(i32 noundef %34, i32 noundef 3, i32 noundef %.0)
  br label %79

38:                                               ; preds = %28
  %39 = sext i32 %.0 to i64
  %40 = getelementptr inbounds i32, i32* %0, i64 %39
  %41 = load i32, i32* %40, align 4
  %42 = load i32, i32* @t, align 4
  %43 = icmp sgt i32 %41, %42
  %44 = zext i1 %43 to i32
  %45 = call i32 @EXEC(i32 noundef %44, i32 noundef 4, i32 noundef %.0)
  %46 = icmp ne i32 %45, 0
  br i1 %46, label %47, label %59

47:                                               ; preds = %38
  br label %48

48:                                               ; preds = %47
  %49 = sext i32 %.0 to i64
  %50 = getelementptr inbounds i32, i32* %1, i64 %49
  %51 = load i32, i32* %50, align 4
  %52 = sext i32 %.0 to i64
  %53 = getelementptr inbounds i32, i32* %0, i64 %52
  %54 = load i32, i32* %53, align 4
  %55 = sub nsw i32 %51, %54
  %56 = load i32, i32* @t, align 4
  %57 = add nsw i32 %55, %56
  store i32 %57, i32* @t, align 4
  %58 = call i32 @EXEC(i32 noundef %57, i32 noundef 5, i32 noundef %.0)
  br label %78

59:                                               ; preds = %38
  br label %60

60:                                               ; preds = %59
  %61 = sext i32 %.0 to i64
  %62 = getelementptr inbounds i32, i32* %0, i64 %61
  %63 = load i32, i32* %62, align 4
  %64 = sext i32 %.0 to i64
  %65 = getelementptr inbounds i32, i32* %1, i64 %64
  %66 = load i32, i32* %65, align 4
  %67 = sub nsw i32 %63, %66
  %68 = load i32, i32* @t, align 4
  %69 = add nsw i32 %67, %68
  store i32 %69, i32* @t, align 4
  %70 = call i32 @EXEC(i32 noundef %69, i32 noundef 6, i32 noundef %.0)
  br label %71

71:                                               ; preds = %60
  %72 = sext i32 %.0 to i64
  %73 = getelementptr inbounds i32, i32* %0, i64 %72
  %74 = load i32, i32* %73, align 4
  %75 = sext i32 %.0 to i64
  %76 = getelementptr inbounds i32, i32* %1, i64 %75
  store i32 %74, i32* %76, align 4
  %77 = call i32 @EXEC(i32 noundef %74, i32 noundef 7, i32 noundef %.0)
  br label %78

78:                                               ; preds = %71, %48
  br label %79

79:                                               ; preds = %78, %31
  %80 = sext i32 %.0 to i64
  %81 = getelementptr inbounds i32, i32* %1, i64 %80
  %82 = load i32, i32* %81, align 4
  %83 = sext i32 %.0 to i64
  %84 = getelementptr inbounds i32, i32* %2, i64 %83
  %85 = load i32, i32* %84, align 4
  %86 = add nsw i32 %82, %85
  %87 = sext i32 %.0 to i64
  %88 = getelementptr inbounds i32, i32* %2, i64 %87
  store i32 %86, i32* %88, align 4
  %89 = call i32 @EXEC(i32 noundef %86, i32 noundef 8, i32 noundef %.0)
  br label %90

90:                                               ; preds = %79
  %91 = add nsw i32 %.0, 1
  br label %4, !llvm.loop !6

92:                                               ; preds = %4
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @print_arr(i32* noundef %0, i32 noundef %1) #0 {
  br label %3

3:                                                ; preds = %10, %2
  %.0 = phi i32 [ 0, %2 ], [ %11, %10 ]
  %4 = icmp slt i32 %.0, %1
  br i1 %4, label %5, label %12

5:                                                ; preds = %3
  %6 = sext i32 %.0 to i64
  %7 = getelementptr inbounds i32, i32* %0, i64 %6
  %8 = load i32, i32* %7, align 4
  %9 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i64 0, i64 0), i32 noundef %8)
  br label %10

10:                                               ; preds = %5
  %11 = add nsw i32 %.0, 1
  br label %3, !llvm.loop !7

12:                                               ; preds = %3
  %13 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([2 x i8], [2 x i8]* @.str.1, i64 0, i64 0))
  ret void
}

declare dso_local i32 @printf(i8* noundef, ...) #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
  %1 = alloca [8 x i32], align 16
  %2 = alloca [8 x i32], align 16
  %3 = alloca [8 x i32], align 16
  %4 = bitcast [8 x i32]* %1 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 16 %4, i8* align 16 bitcast ([8 x i32]* @__const.main.a to i8*), i64 32, i1 false)
  %5 = bitcast [8 x i32]* %2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 16 %5, i8* align 16 bitcast ([8 x i32]* @__const.main.b to i8*), i64 32, i1 false)
  %6 = bitcast [8 x i32]* %3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 16 %6, i8* align 16 bitcast ([8 x i32]* @__const.main.c to i8*), i64 32, i1 false)
  %7 = getelementptr inbounds [8 x i32], [8 x i32]* %1, i64 0, i64 0
  %8 = getelementptr inbounds [8 x i32], [8 x i32]* %2, i64 0, i64 0
  %9 = getelementptr inbounds [8 x i32], [8 x i32]* %3, i64 0, i64 0
  call void @zsy_test(i32* noundef %7, i32* noundef %8, i32* noundef %9)
  %10 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([6 x i8], [6 x i8]* @.str.2, i64 0, i64 0))
  %11 = getelementptr inbounds [8 x i32], [8 x i32]* %3, i64 0, i64 0
  call void @print_arr(i32* noundef %11, i32 noundef 8)
  br label %12

12:                                               ; preds = %20, %0
  %.0 = phi i32 [ 0, %0 ], [ %21, %20 ]
  %13 = icmp slt i32 %.0, 8
  br i1 %13, label %14, label %22

14:                                               ; preds = %12
  %15 = add nsw i32 %.0, 1
  %16 = add nsw i32 %.0, 1
  ;%16 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([10 x i8], [10 x i8]* @.str.3, i64 0, i64 0), i32 noundef %15)
  %17 = sext i32 %.0 to i64
  %18 = getelementptr inbounds [8 x [8 x i32]], [8 x [8 x i32]]* @exec_var, i64 0, i64 %17
  %19 = getelementptr inbounds [8 x i32], [8 x i32]* %18, i64 0, i64 0
  ;call void @print_arr(i32* noundef %19, i32 noundef 8)
  br label %20

20:                                               ; preds = %14
  %21 = add nsw i32 %.0, 1
  br label %12, !llvm.loop !8

22:                                               ; preds = %12
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #2

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { argmemonly nofree nounwind willreturn }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 14.0.6 (https://github.com/llvm/llvm-project.git f28c006a5895fc0e329fe15fead81e37457cb1d1)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
!7 = distinct !{!7, !5}
!8 = distinct !{!8, !5}
