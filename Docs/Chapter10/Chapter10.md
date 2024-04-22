# Overview

ELF SUPPORT

# 细节



- CMakeLists.txt
- Cpu0InstrInfo.td 
- Disassembler/CMakeLists.txt 
- Disassembler/Cpu0Disassembler.cpp
- Cpu0Subtarget.h



共计5个文件




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




