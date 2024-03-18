# Overview

支持其他数据类型


# 细节

- Cpu0Subtarget.h
- Cpu0InstrInfo.td
- Cpu0ISelDAGToDAG.cpp
- Cpu0ISelLowering.cpp
- Cpu0ISelLowering.h
- Cpu0SEISelDAGToDAG.cpp
- Cpu0SEISelDAGToDAG.h




共计7个文件




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



进入到Cpu0Test目录下，以ch7_1_vector.cpp为例，输入以下命令
```shell
./../llvm-project-llvmorg-17.0.6/build/bin/clang -target mips-unknown-linux-gnu -c  ch7_1_vector.cpp -emit-llvm -o ch7_1_vector.bc
./../llvm-project-llvmorg-17.0.6/build/bin/llc -march=cpu0 -relocation-model=pic -filetype=asm ch7_1_vector.bc -o - 
```

