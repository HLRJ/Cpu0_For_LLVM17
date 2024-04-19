//===-- Cpu0MachineFunctionInfo.cpp - Private data used for Cpu0 ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Cpu0MachineFunction.h"

#include "Cpu0InstrInfo.h"
#include "Cpu0Subtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Function.h"

using namespace llvm;

bool FixGlobalBaseReg;

MachineFunctionInfo *
Cpu0FunctionInfo::clone(BumpPtrAllocator &Allocator, MachineFunction &DestMF,
                        const DenseMap<MachineBasicBlock *, MachineBasicBlock *>
                        &Src2DstMBB) const {
    return DestMF.cloneInfo<Cpu0FunctionInfo>(*this);
}
Cpu0FunctionInfo::~Cpu0FunctionInfo() = default;

bool Cpu0FunctionInfo::globalBaseRegFixed() const {
  return FixGlobalBaseReg;
}

bool Cpu0FunctionInfo::globalBaseRegSet() const {
  return GlobalBaseReg;
}

unsigned Cpu0FunctionInfo::getGlobalBaseReg() {
  return GlobalBaseReg = Cpu0::GP;
}

void Cpu0FunctionInfo::createEhDataRegsFI(MachineFunction &MF) {
  const TargetRegisterInfo &TRI = *MF.getSubtarget().getRegisterInfo();
  for (int I = 0; I < 2; ++I) {
    const TargetRegisterClass &RC = Cpu0::CPURegsRegClass;

    EhDataRegFI[I] = MF.getFrameInfo().CreateStackObject(
        TRI.getSpillSize(RC), TRI.getSpillAlign(RC), false);
  }
}


MachinePointerInfo Cpu0FunctionInfo::callPtrInfo(MachineFunction &MF, const char *ES) {
  return MachinePointerInfo(MF.getPSVManager().getExternalSymbolCallEntry(ES));
}

MachinePointerInfo Cpu0FunctionInfo::callPtrInfo(MachineFunction &MF, const GlobalValue *GV) {
  return MachinePointerInfo(MF.getPSVManager().getGlobalValueCallEntry(GV));
}

void Cpu0FunctionInfo::anchor() { }

