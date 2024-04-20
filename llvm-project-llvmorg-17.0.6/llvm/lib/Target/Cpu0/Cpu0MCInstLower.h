//===-- Cpu0MCInstLower.h - Lower MachineInstr to MCInst -------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CPU0_CPU0MCINSTLOWER_H
#define LLVM_LIB_TARGET_CPU0_CPU0MCINSTLOWER_H


#include "MCTargetDesc/Cpu0MCExpr.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
  class MCContext;
  class MCInst;
  class MCOperand;
  class MachineInstr;
  class MachineFunction;
  class Cpu0AsmPrinter;

//@1 {
/// This class is used to lower an MachineInstr into an MCInst.
class LLVM_LIBRARY_VISIBILITY Cpu0MCInstLower {
//@2
  typedef MachineOperand::MachineOperandType MachineOperandType;
  MCContext *Ctx;
  Cpu0AsmPrinter &AsmPrinter;
public:
  Cpu0MCInstLower(Cpu0AsmPrinter &asmprinter);
  void Initialize(MCContext* C);
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;
  MCOperand LowerOperand(const MachineOperand& MO, unsigned offset = 0) const;

  void LowerCPLOAD(SmallVector<MCInst, 4>& MCInsts);
#ifdef ENABLE_GPRESTORE
  void LowerCPRESTORE(int64_t Offset, SmallVector<MCInst, 4>& MCInsts);
#endif
private:
  MCOperand LowerSymbolOperand(const MachineOperand &MO,
                               MachineOperandType MOTy, unsigned Offset) const;

  MCOperand createSub(MachineBasicBlock *BB1, MachineBasicBlock *BB2,
                      Cpu0MCExpr::Cpu0ExprKind Kind) const;
  void lowerLongBranchLUi(const MachineInstr *MI, MCInst &OutMI) const;
  void lowerLongBranchADDiu(const MachineInstr *MI, MCInst &OutMI,
                            int Opcode,
                            Cpu0MCExpr::Cpu0ExprKind Kind) const;
  bool lowerLongBranch(const MachineInstr *MI, MCInst &OutMI) const;
};
}

#endif

