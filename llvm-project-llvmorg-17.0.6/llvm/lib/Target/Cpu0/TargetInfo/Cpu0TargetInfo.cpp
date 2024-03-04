//===-- Cpu0TargetInfo.cpp - Cpu0 Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//


#include "Cpu0TargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;


Target &llvm::getTheCpu0Target() {
  static Target TheCpu0Target;
  return TheCpu0Target;
}
Target &llvm::getTheCpu0elTarget() {
  static Target TheCpu0elTarget;
  return TheCpu0elTarget;
}


extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCpu0TargetInfo() {
  RegisterTarget<Triple::cpu0,
      /*HasJIT=*/true> X(getTheCpu0Target(), "cpu0", "CPU0 (32-bit big endian)", "Cpu0");

  RegisterTarget<Triple::cpu0el,
      /*HasJIT=*/true> Y(getTheCpu0elTarget(), "cpu0el", "CPU0 (32-bit little endian)", "Cpu0");
}

