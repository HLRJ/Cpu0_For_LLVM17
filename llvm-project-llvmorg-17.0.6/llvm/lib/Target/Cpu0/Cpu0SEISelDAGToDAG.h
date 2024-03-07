//===-- Cpu0SEISelDAGToDAG.h - A Dag to Dag Inst Selector for Cpu0SE -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of Cpu0DAGToDAGISel specialized for cpu032.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEISELDAGTODAG_H
#define LLVM_LIB_TARGET_CPU0_CPU0SEISELDAGTODAG_H



#include "Cpu0ISelDAGToDAG.h"

namespace llvm {

  class Cpu0SEDAGToDAGISel : public Cpu0DAGToDAGISel {

  public:
    explicit Cpu0SEDAGToDAGISel(Cpu0TargetMachine &TM, CodeGenOpt::Level OL)
        : Cpu0DAGToDAGISel(TM, OL) {}

  private:

    bool runOnMachineFunction(MachineFunction &MF) override;

    bool trySelect(SDNode *Node) override;

    void processFunctionAfterISel(MachineFunction &MF) override;

    // Insert instructions to initialize the global base register in the
    // first MBB of the function.
//  void initGlobalBaseReg(MachineFunction &MF);

  };

  FunctionPass *createCpu0SEISelDag(Cpu0TargetMachine &TM,
                                    CodeGenOpt::Level OptLevel);

}

#endif

