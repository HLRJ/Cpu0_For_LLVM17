//===-- Cpu0TargetInfo.h - Cpu0 Target Implementation -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MIPS_TARGETINFO_CPU0TARGETINFO_H
#define LLVM_LIB_TARGET_MIPS_TARGETINFO_CPU0TARGETINFO_H

namespace llvm {

  class Target;

  Target &getTheCpu0Target();
  Target &getTheCpu0elTarget();
//Target &getTheCpu064Target();
//Target &getTheCpu064elTarget();

} // namespace llvm

#endif // LLVM_LIB_TARGET_MIPS_TARGETINFO_MIPSTARGETINFO_H
