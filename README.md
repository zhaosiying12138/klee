# PDG AutoVectorization  
A toy auto-vectoraization implementation about the application of **Program Dependence Graph** using LLVM by zhaosiying12138.  
It can do auto-vectoraization when **control flow exists** since its **If-Conversion** algorithm can treat control dependence as well as data dependence.

## 1. File Organization  
lib/Module/PDGAnalysis.cpp: an LLVM Analysis Pass to generate PDG information from LLVM IR  
lib/Module/Tarjan_SCC.cpp: an utility class to calculate SCC of PDG  
lib/Core/PDGExecutor.cpp: modification on klee executor to executor LLVM IR in a loop distribution form automatically  
zsy_test/demo2\_3.ll & demo3\_3.ll: the demo LLVM IR to be executed  

## 2. Usage  
### Step 1: Make a testfile (If you want to run your own testcode)  
```
clang -Xclang -disable-O0-optnone -S -O0 -emit-llvm demo1.c -o demo1_1.ll  
opt -S -mem2reg demo1_1.ll -o demo1_2.ll  
opt -S -loop-simplify demo1_2.ll -o demo1_3.ll  
```

### Step 2: Build modified KLEE  
```
mkdir build  
cd build  
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_SOLVER_Z3=ON -DZ3_INCLUDE_DIRS=/your-z3-dir/include/ -DZ3_LIBRARIES=/your-z3-dir/lib/libz3.so -DLLVM_DIR=/your-llvm-dir/  
make -j 65535  
```

### Step 3: Run the LLVM IR using KLEE  
```
build/bin/klee zsy_test/demo2_3.ll  
```

## 3. Experiment Result  
![Program Dependence Graph Demo](./demo/zsy_test_cdg_demo.png)  
**[TODO!!!]**   

## 4. Link  
[《零基础入门控制依赖图构建的理论与实践》](https://zhuanlan.zhihu.com/p/658705992)  

## 5. Copyright  
Copyright (c) 2023 By 流月城先进偃甲技术研究院-对伏羲外包国家重点实验室-流霞祭司曌鹓鶵 founded by 五色石炼制关键技术天界自然科学基金(2022LYC12138).  All rights reserved.  
