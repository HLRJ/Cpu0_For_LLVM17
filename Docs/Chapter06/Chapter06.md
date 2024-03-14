# Overview

支持全局变量


# 细节

- Cpu0Subtarget.h
- Cpu0Subtarget.cpp
- MCTargetDesc/Cpu0BaseInfo.h
- MCTargetDesc/Cpu0MCCodeEmitter.cpp
- Cpu0TargetObjectFile.h
- Cpu0TargetObjectFile.cpp
- Cpu0RegisterInfo.cpp
- Cpu0ISelLowering.h
- Cpu0ISelLowering.cpp
- Cpu0ISelDAGToDAG.h
- Cpu0ISelDAGToDAG.cpp
- Cpu0InstrInfo.td
- Cpu0MachineFunction.h
- Cpu0MachineFunction.cpp
- Cpu0AsmPrinter.cpp
- Cpu0MCInstLower.h
- Cpu0MCInstLower.cpp




共计17个文件




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



进入到Cpu0Test目录下，我们使用ch4_math.ll，输入以下命令
```shell
./../llvm-project-llvmorg-17.0.6/build/bin/clang -target mips-unknown-linux-gnu -c  ch4_1_math.cpp -emit-llvm -o ch6_1.bc
./../llvm-project-llvmorg-17.0.6/build/bin/llc -march=cpu0 -relocation-model=pic -cpu0-use-small-section=true -filetype=asm -debug ch6_1.bc -o -
./../llvm-project-llvmorg-17.0.6/build/bin/llc -march=cpu0 -relocation-model=pic -filetype=obj ch6_1.bc -o ch6_1.cpu0.o
objdump -s ch6_1.cpu0.o
```

