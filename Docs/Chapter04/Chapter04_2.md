# Overview

To support logic instructions


# 细节

- Cpu0InstrInfo.td
- Cpu0ISelLowering.cpp
- Cpu0Subtarget.h





共计3个文件




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



进入到Cpu0Test目录下，我们使用ch4_2_slt_explain.cpp
```shell
./../llvm-project-llvmorg-17.0.6/build/bin/clang -target mips-unknown-linux-gnu -c  ch4_2_slt_explain.cpp -emit-llvm -o ch4_2_slt_explain.bc
./../llvm-project-llvmorg-17.0.6/build/bin/llc -march=cpu0 -mcpu=cpu032I -relocation-model=static -filetype=asm ch4_2_slt_explain.bc -o -
```

结果如下：

![](./images/img04_2_01.png)




和书上一致。其他文件自行测试。



# note

上次commit提交把这个04_2文件也提交了，失误。
