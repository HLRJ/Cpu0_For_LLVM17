//===-- Cpu0MachineFunctionInfo.h - Private data used for Cpu0 ----*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Cpu0 specific subclass of MachineFunctionInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CPU0_CPU0MACHINEFUNCTION_H
#define LLVM_LIB_TARGET_CPU0_CPU0MACHINEFUNCTION_H



#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/Target/TargetMachine.h"
#include <map>

namespace llvm {

//@1 {
/// Cpu0FunctionInfo - This class is derived from MachineFunction private
/// Cpu0 target-specific information for each MachineFunction.
class Cpu0FunctionInfo : public MachineFunctionInfo {
public:
//  Cpu0FunctionInfo(MachineFunction& MF)
//  : MF(MF),
//    VarArgsFrameIndex(0),
//    EmitNOAT(false),
//    MaxCallFrameSize(0)
//    {}

  Cpu0FunctionInfo(const Function &F, const TargetSubtargetInfo *STI) {}
  MachineFunctionInfo *
  clone(BumpPtrAllocator &Allocator, MachineFunction &DestMF,
        const DenseMap<MachineBasicBlock *, MachineBasicBlock *> &Src2DstMBB)
      const override;
  ~Cpu0FunctionInfo() override;

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

  bool getEmitNOAT() const { return EmitNOAT; }
  void setEmitNOAT() { EmitNOAT = true; }
private:
  virtual void anchor();

//  MachineFunction& MF;

    /// VarArgsFrameIndex - FrameIndex for start of varargs area.
  int VarArgsFrameIndex = 0;

  bool EmitNOAT = false;
  unsigned MaxCallFrameSize = 0;
};
//@1 }

} // end of namespace llvm

#endif // CPU0_MACHINE_FUNCTION_INFO_H

