//===-- Cpu0ISelLowering.cpp - Cpu0 DAG Lowering Implementation -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that Cpu0 uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//
#include "Cpu0ISelLowering.h"

#include "Cpu0MachineFunction.h"
#include "Cpu0TargetMachine.h"
#include "Cpu0TargetObjectFile.h"
#include "Cpu0Subtarget.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0-lower"

SDValue Cpu0TargetLowering::getGlobalReg(SelectionDAG &DAG, EVT Ty) const {
  Cpu0FunctionInfo *FI = DAG.getMachineFunction().getInfo<Cpu0FunctionInfo>();
  return DAG.getRegister(FI->getGlobalBaseReg(), Ty);
}

//@getTargetNode(GlobalAddressSDNode
SDValue Cpu0TargetLowering::getTargetNode(GlobalAddressSDNode *N, EVT Ty,
                                          SelectionDAG &DAG,
                                          unsigned Flag) const {
  return DAG.getTargetGlobalAddress(N->getGlobal(), SDLoc(N), Ty, 0, Flag);
}

//@getTargetNode(ExternalSymbolSDNode
SDValue Cpu0TargetLowering::getTargetNode(ExternalSymbolSDNode *N, EVT Ty,
                                          SelectionDAG &DAG,
                                          unsigned Flag) const {
  return DAG.getTargetExternalSymbol(N->getSymbol(), Ty, Flag);
}

//@3_1 1 {
const char *Cpu0TargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case Cpu0ISD::JmpLink:           return "Cpu0ISD::JmpLink";
  case Cpu0ISD::TailCall:          return "Cpu0ISD::TailCall";
  case Cpu0ISD::Hi:                return "Cpu0ISD::Hi";
  case Cpu0ISD::Lo:                return "Cpu0ISD::Lo";
  case Cpu0ISD::GPRel:             return "Cpu0ISD::GPRel";
  case Cpu0ISD::Ret:               return "Cpu0ISD::Ret";
  case Cpu0ISD::EH_RETURN:         return "Cpu0ISD::EH_RETURN";
  case Cpu0ISD::DivRem:            return "Cpu0ISD::DivRem";
  case Cpu0ISD::DivRemU:           return "Cpu0ISD::DivRemU";
  case Cpu0ISD::Wrapper:           return "Cpu0ISD::Wrapper";
  default:                         return NULL;
  }
}
//@3_1 1 }

//@Cpu0TargetLowering {
Cpu0TargetLowering::Cpu0TargetLowering(const Cpu0TargetMachine &TM,
                                       const Cpu0Subtarget &STI)
    : TargetLowering(TM), Subtarget(STI), ABI(TM.getABI()) {

  // Cpu0 does not have i1 type, so use i32 for
  // setcc operations results (slt, sgt, ...).
  setBooleanContents(ZeroOrOneBooleanContent);
  setBooleanVectorContents(ZeroOrNegativeOneBooleanContent);

  // Load extented operations for i1 types must be promoted
  for (MVT VT : MVT::integer_valuetypes()) {
    setLoadExtAction(ISD::EXTLOAD,  VT, MVT::i1,  Promote);
    setLoadExtAction(ISD::ZEXTLOAD, VT, MVT::i1,  Promote);
    setLoadExtAction(ISD::SEXTLOAD, VT, MVT::i1,  Promote);
  }

  // Cpu0 Custom Operations
  setOperationAction(ISD::GlobalAddress,      MVT::i32,   Custom);

  // Handle i64 shl
  setOperationAction(ISD::SHL_PARTS,          MVT::i32,   Expand);
  setOperationAction(ISD::SRA_PARTS,          MVT::i32,   Expand);
  setOperationAction(ISD::SRL_PARTS,          MVT::i32,   Expand);

  // Cpu0 Custom Operations
  setOperationAction(ISD::SDIV, MVT::i32, Expand);
  setOperationAction(ISD::SREM, MVT::i32, Expand);
  setOperationAction(ISD::UDIV, MVT::i32, Expand);
  setOperationAction(ISD::UREM, MVT::i32, Expand);
  // Operations not directly supported by Cpu0.
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i1 , Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i8 , Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i16 , Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i32 , Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::Other , Expand);


  setTargetDAGCombine(ISD::SDIVREM);
  setTargetDAGCombine(ISD::UDIVREM);
//- Set .align 2
// It will emit .align 2 later
  setMinFunctionAlignment(Align(2));

}

const Cpu0TargetLowering *Cpu0TargetLowering::create(const Cpu0TargetMachine &TM,
                                                     const Cpu0Subtarget &STI) {
  return llvm::createCpu0SETargetLowering(TM, STI);
}

static SDValue performDivRemCombine(SDNode *N, SelectionDAG& DAG,
                                    TargetLowering::DAGCombinerInfo &DCI,
                                    const Cpu0Subtarget &Subtarget) {
  if (DCI.isBeforeLegalizeOps())
    return SDValue();

  EVT Ty = N->getValueType(0);
  unsigned LO = Cpu0::LO;
  unsigned HI = Cpu0::HI;
  unsigned Opc = N->getOpcode() == ISD::SDIVREM ? Cpu0ISD::DivRem :
                 Cpu0ISD::DivRemU;
  SDLoc DL(N);

  SDValue DivRem = DAG.getNode(Opc, DL, MVT::Glue,
                               N->getOperand(0), N->getOperand(1));
  SDValue InChain = DAG.getEntryNode();
  SDValue InGlue = DivRem;

  // insert MFLO
  if (N->hasAnyUseOfValue(0)) {
    SDValue CopyFromLo = DAG.getCopyFromReg(InChain, DL, LO, Ty,
                                            InGlue);
    DAG.ReplaceAllUsesOfValueWith(SDValue(N, 0), CopyFromLo);
    InChain = CopyFromLo.getValue(1);
    InGlue = CopyFromLo.getValue(2);
  }

  // insert MFHI
  if (N->hasAnyUseOfValue(1)) {
    SDValue CopyFromHi = DAG.getCopyFromReg(InChain, DL,
                                            HI, Ty, InGlue);
    DAG.ReplaceAllUsesOfValueWith(SDValue(N, 1), CopyFromHi);
  }

  return SDValue();
}


EVT Cpu0TargetLowering::getSetCCResultType(const DataLayout &, LLVMContext &,
                                           EVT VT) const {
  if (!VT.isVector())
    return MVT::i32;
  return VT.changeVectorElementTypeToInteger();
}

SDValue Cpu0TargetLowering::PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI)
const {
  SelectionDAG &DAG = DCI.DAG;
  unsigned Opc = N->getOpcode();

  switch (Opc) {
    default: break;
    case ISD::SDIVREM:
    case ISD::UDIVREM:
      return performDivRemCombine(N, DAG, DCI, Subtarget);
  }

  return SDValue();
}

SDValue Cpu0TargetLowering::
LowerOperation(SDValue Op, SelectionDAG &DAG) const
{
  switch (Op.getOpcode())
  {
  case ISD::GlobalAddress:      return lowerGlobalAddress(Op, DAG);
  }
  return SDValue();
}


//===----------------------------------------------------------------------===//
//  Lower helper functions
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Misc Lower Operation implementation
//===----------------------------------------------------------------------===//


SDValue Cpu0TargetLowering::lowerGlobalAddress(SDValue Op,
                                               SelectionDAG &DAG) const {
  //@lowerGlobalAddress }
  SDLoc DL(Op);
  const Cpu0TargetObjectFile *TLOF =
        static_cast<const Cpu0TargetObjectFile *>(
            getTargetMachine().getObjFileLowering());
  //@lga 1 {
  EVT Ty = Op.getValueType();
  GlobalAddressSDNode *N = cast<GlobalAddressSDNode>(Op);
  const GlobalValue *GV = N->getGlobal();
  //@lga 1 }

  if (!isPositionIndependent()) {
    //@ %gp_rel relocation
    const GlobalObject *GO = GV->getAliaseeObject();
    if (GO && TLOF->IsGlobalInSmallSection(GO, getTargetMachine())) {
      SDValue GA = DAG.getTargetGlobalAddress(GV, DL, MVT::i32, 0,
                                              Cpu0II::MO_GPREL);
      SDValue GPRelNode = DAG.getNode(Cpu0ISD::GPRel, DL,
                                      DAG.getVTList(MVT::i32), GA);
      SDValue GPReg = DAG.getRegister(Cpu0::GP, MVT::i32);
      return DAG.getNode(ISD::ADD, DL, MVT::i32, GPReg, GPRelNode);
    }

    //@ %hi/%lo relocation
    return getAddrNonPIC(N, Ty, DAG);
  }

  if (GV->hasInternalLinkage() || (GV->hasLocalLinkage() && !isa<Function>(GV)))
    return getAddrLocal(N, Ty, DAG);

  //@large section
  const GlobalObject *GO = GV->getAliaseeObject();
  if (GO && !TLOF->IsGlobalInSmallSection(GO, getTargetMachine()))
    return getAddrGlobalLargeGOT(
        N, Ty, DAG, Cpu0II::MO_GOT_HI16, Cpu0II::MO_GOT_LO16,
        DAG.getEntryNode(),
        MachinePointerInfo::getGOT(DAG.getMachineFunction()));
  return getAddrGlobal(
      N, Ty, DAG, Cpu0II::MO_GOT, DAG.getEntryNode(),
      MachinePointerInfo::getGOT(DAG.getMachineFunction()));
}



#include "Cpu0GenCallingConv.inc"

//===----------------------------------------------------------------------===//
//@            Formal Arguments Calling Convention Implementation
//===----------------------------------------------------------------------===//

//@LowerFormalArguments {
/// LowerFormalArguments - transform physical registers into virtual registers
/// and generate load operations for arguments places on the stack.
SDValue
Cpu0TargetLowering::LowerFormalArguments(SDValue Chain,
                                         CallingConv::ID CallConv,
                                         bool IsVarArg,
                                         const SmallVectorImpl<ISD::InputArg> &Ins,
                                         const SDLoc &DL, SelectionDAG &DAG,
                                         SmallVectorImpl<SDValue> &InVals)
                                          const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  Cpu0FunctionInfo *Cpu0FI = MF.getInfo<Cpu0FunctionInfo>();

  Cpu0FI->setVarArgsFrameIndex(0);

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(),
                 ArgLocs, *DAG.getContext());
  Cpu0CC Cpu0CCInfo(CallConv, ABI.IsO32(),
                    CCInfo);

  Cpu0FI->setFormalArgInfo(CCInfo.getStackSize(),
                           Cpu0CCInfo.hasByValArg());

  return Chain;
}
// @LowerFormalArguments }

//===----------------------------------------------------------------------===//
//@              Return Value Calling Convention Implementation
//===----------------------------------------------------------------------===//

SDValue
Cpu0TargetLowering::LowerReturn(SDValue Chain,
                                CallingConv::ID CallConv, bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &DL, SelectionDAG &DAG) const {
  // CCValAssign - represent the assignment of
  // the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;
  MachineFunction &MF = DAG.getMachineFunction();

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, MF, RVLocs,
                 *DAG.getContext());
  Cpu0CC Cpu0CCInfo(CallConv, ABI.IsO32(),
                    CCInfo);

  // Analyze return values.
  Cpu0CCInfo.analyzeReturn(Outs, Subtarget.abiUsesSoftFloat(),
                           MF.getFunction().getReturnType());

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    SDValue Val = OutVals[i];
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    if (RVLocs[i].getValVT() != RVLocs[i].getLocVT())
      Val = DAG.getNode(ISD::BITCAST, DL, RVLocs[i].getLocVT(), Val);

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

//@Ordinary struct type: 2 {
  // The cpu0 ABIs for returning structs by value requires that we copy
  // the sret argument into $v0 for the return. We saved the argument into
  // a virtual register in the entry block, so now we copy the value out
  // and into $v0.
  if (MF.getFunction().hasStructRetAttr()) {
    Cpu0FunctionInfo *Cpu0FI = MF.getInfo<Cpu0FunctionInfo>();
    unsigned Reg = Cpu0FI->getSRetReturnReg();

    if (!Reg)
      llvm_unreachable("sret virtual register not created in the entry block");
    SDValue Val =
        DAG.getCopyFromReg(Chain, DL, Reg, getPointerTy(DAG.getDataLayout()));
    unsigned V0 = Cpu0::V0;

    Chain = DAG.getCopyToReg(Chain, DL, V0, Val, Flag);
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(V0, getPointerTy(DAG.getDataLayout())));
  }
//@Ordinary struct type: 2 }

  RetOps[0] = Chain;  // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  // Return on Cpu0 is always a "ret $lr"
  return DAG.getNode(Cpu0ISD::Ret, DL, MVT::Other, RetOps);
}

bool
Cpu0TargetLowering::isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const {
  // The Cpu0 target isn't yet aware of offsets.
  return false;
}

Cpu0TargetLowering::Cpu0CC::Cpu0CC(
  CallingConv::ID CC, bool IsO32_, CCState &Info,
  Cpu0CC::SpecialCallingConvType SpecialCallingConv_)
  : CCInfo(Info), CallConv(CC), IsO32(IsO32_) {
  // Pre-allocate reserved argument area.
  CCInfo.AllocateStack(reservedArgArea(), Align(1));
}

template<typename Ty>
void Cpu0TargetLowering::Cpu0CC::
analyzeReturn(const SmallVectorImpl<Ty> &RetVals, bool IsSoftFloat,
              const SDNode *CallNode, const Type *RetTy) const {
  CCAssignFn *Fn;

  Fn = RetCC_Cpu0;

  for (unsigned I = 0, E = RetVals.size(); I < E; ++I) {
    MVT VT = RetVals[I].VT;
    ISD::ArgFlagsTy Flags = RetVals[I].Flags;
    MVT RegVT = this->getRegVT(VT, IsSoftFloat);

    if (Fn(I, VT, RegVT, CCValAssign::Full, Flags, this->CCInfo)) {
#ifndef NDEBUG
      dbgs() << "Call result #" << I << " has unhandled type "
             << EVT(VT).getEVTString() << '\n';
#endif
      llvm_unreachable(nullptr);
    }
  }
}

void Cpu0TargetLowering::Cpu0CC::
analyzeCallResult(const SmallVectorImpl<ISD::InputArg> &Ins, bool IsSoftFloat,
                  const SDNode *CallNode, const Type *RetTy) const {
  analyzeReturn(Ins, IsSoftFloat, CallNode, RetTy);
}

void Cpu0TargetLowering::Cpu0CC::
analyzeReturn(const SmallVectorImpl<ISD::OutputArg> &Outs, bool IsSoftFloat,
              const Type *RetTy) const {
  analyzeReturn(Outs, IsSoftFloat, nullptr, RetTy);
}

unsigned Cpu0TargetLowering::Cpu0CC::reservedArgArea() const {
  return (IsO32 && (CallConv != CallingConv::Fast)) ? 8 : 0;
}

MVT Cpu0TargetLowering::Cpu0CC::getRegVT(MVT VT,
                                         bool IsSoftFloat) const {
  if (IsSoftFloat || IsO32)
    return VT;

  return VT;
}
