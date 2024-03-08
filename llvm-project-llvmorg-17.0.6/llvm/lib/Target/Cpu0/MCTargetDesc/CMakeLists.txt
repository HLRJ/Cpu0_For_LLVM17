# MCTargetDesc/CMakeLists.txt
add_llvm_component_library(LLVMCpu0Desc
  Cpu0ABIInfo.cpp
  Cpu0MCTargetDesc.cpp
  Cpu0MCAsmInfo.cpp

  LINK_COMPONENTS
  MC
  Cpu0Info
  Support
  TargetParser
  Cpu0AsmPrinter

  ADD_TO_COMPONENT
  Cpu0
  )

