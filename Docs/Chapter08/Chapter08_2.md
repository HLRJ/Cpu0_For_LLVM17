# Overview

控制流语句


# 细节

- MCTargetDesc/Cpu0FixupKinds.h
- MCTargetDesc/Cpu0MCCodeEmitter.cpp
- MCTargetDesc/Cpu0AsmBackend.cpp
- CMakeLists.txt
- Cpu0.h
- Cpu0AsmPrinter.cpp
- Cpu0AsmPrinter.h
- Cpu0CondMov.td
- Cpu0DelaySlotFiller.cpp
- Cpu0DelUselessJMP.cpp
- Cpu0InstrInfo.h
- Cpu0InstrInfo.td
- Cpu0ISelLowering.h
- Cpu0ISelLowering.cpp
- Cpu0LongBranch.cpp
- Cpu0MCInstLower.cpp
- Cpu0MCInstLower.h
- Cpu0SEInstrInfo.cpp
- Cpu0SEInstrInfo.h
- Cpu0Subtarget.h
- Cpu0TargetMachine.cpp
- 






共计21个文件




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



