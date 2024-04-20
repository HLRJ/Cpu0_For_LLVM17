# Overview

函数调用

# 细节


- Cpu0.h
- CMakeLists.txt
- Cpu0AsmPrinter.cpp
- Cpu0EmitGPRestore.cpp
- Cpu0InstrInfo.td
- Cpu0ISelLowering.cpp
- Cpu0ISelLowering.h
- Cpu0MachineFunction.h
- Cpu0MCInstLower.cpp
- Cpu0MCInstLower.h
- Cpu0RegisterInfo.cpp
- Cpu0SEFrameLowering.cpp
- Cpu0SEInstrInfo.cpp
- Cpu0SEInstrInfo.h
- Cpu0Subtarget.h
- Cpu0TargetMachine.cpp






共计16个文件




# 编译

依旧是在build目录下

```shell
cmake -DCMAKE_BUILD_TYPE=Debug         \
        -DBUILD_SHARED_LIBS=ON         \
        -DLLVM_USE_LINKER=lld          \
        -DLLVM_ENABLE_PROJECTS="clang" \
        -DLLVM_TARGETS_TO_BUILD="Mips;Cpu0"   \
        -DLLVM_OPTIMIZED_TABLEGEN=ON   \
        -GNinja ../llvm && ninja
```






