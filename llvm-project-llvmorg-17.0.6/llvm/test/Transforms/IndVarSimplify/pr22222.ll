; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -passes=indvars -S < %s | FileCheck %s

@b = common global i32 0, align 4
@c = common global i32 0, align 4
@a = common global i32 0, align 4

declare void @abort() #1

; Function Attrs: nounwind ssp uwtable
define i32 @main() {
; CHECK-LABEL: @main(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br label [[FOR_COND1_PREHEADER:%.*]]
; CHECK:       for.cond1.preheader:
; CHECK-NEXT:    br label [[FOR_BODY3:%.*]]
; CHECK:       for.body3:
; CHECK-NEXT:    br i1 false, label [[FOR_BODY3]], label [[FOR_END:%.*]]
; CHECK:       for.end:
; CHECK-NEXT:    br i1 false, label [[FOR_COND1_PREHEADER]], label [[FOR_END6:%.*]]
; CHECK:       for.end6:
; CHECK-NEXT:    [[OR_LCSSA_LCSSA:%.*]] = phi i32 [ -1, [[FOR_END]] ]
; CHECK-NEXT:    store i32 [[OR_LCSSA_LCSSA]], ptr @a, align 4
; CHECK-NEXT:    [[CMP7:%.*]] = icmp eq i32 [[OR_LCSSA_LCSSA]], -1
; CHECK-NEXT:    br i1 [[CMP7]], label [[IF_END:%.*]], label [[IF_THEN:%.*]]
; CHECK:       if.then:
; CHECK-NEXT:    tail call void @abort()
; CHECK-NEXT:    unreachable
; CHECK:       if.end:
; CHECK-NEXT:    ret i32 0
;
entry:
  %a.promoted13 = load i32, ptr @a, align 4
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.end
  %or.lcssa14 = phi i32 [ %a.promoted13, %entry ], [ %or.lcssa, %for.end ]
  %d.010 = phi i32 [ 1, %entry ], [ 0, %for.end ]
  br label %for.body3

for.body3:                                        ; preds = %for.cond1.preheader, %for.body3
  %inc12 = phi i32 [ 0, %for.cond1.preheader ], [ %inc, %for.body3 ]
  %or11 = phi i32 [ %or.lcssa14, %for.cond1.preheader ], [ %or, %for.body3 ]
  %add = sub i32 %inc12, %d.010
  %or = or i32 %or11, %add
  %inc = add i32 %inc12, 1
  br i1 false, label %for.body3, label %for.end

for.end:                                          ; preds = %for.body3
  %or.lcssa = phi i32 [ %or, %for.body3 ]
  br i1 false, label %for.cond1.preheader, label %for.end6

for.end6:                                         ; preds = %for.end
  %or.lcssa.lcssa = phi i32 [ %or.lcssa, %for.end ]
  store i32 %or.lcssa.lcssa, ptr @a, align 4
  %cmp7 = icmp eq i32 %or.lcssa.lcssa, -1
  br i1 %cmp7, label %if.end, label %if.then

if.then:                                          ; preds = %for.end6
  tail call void @abort() #2
  unreachable

if.end:                                           ; preds = %for.end6
  ret i32 0
}