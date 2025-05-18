# 功能
spf(simple bpf)是一款轻量的ebpf运行时，可以在用户态、嵌入式、内核等多种环境运行。支持多种方式运行ebpf：字节码解释执行、jit成本机指令执行、编译为SPF/BARE文件运行。  

# 特点
mybpf的特点  
  1. 轻量：runtime极度轻量，能以极低的代价到处移植  
  2. 兼容：兼容多种运行环境，包括嵌入式、内核、用户态、跨OS、跨硬件平台  
  3. 灵活：APP开发难度低，部署灵活快速，可以快上快下，可以动态加载、卸载APP  
  4. 高性能：可以编译为本地指令执行  
  5. 节省空间：BARE和SPF格式文件非常小，占用空间小  

# 架构
spf主要分为两部分： 编译工具 + runtime。  
编译工具支持将ebpf文件编译为SPF/BARE文件  
runtime负责运行SPF/BARE文件  

# 文件格式
当前支持两种目标指令集：ARM64和X86-64，其它还待增加。  
支持输出两种不同的文件格式：SPF格式和BARE格式。  
BARE格式较简单，支持bss全局变量(不支持data, rodata),  支持内部子函数、支持helper。不支持map。  
SPF格式比BARE格式复杂(但也比elf要简单)，支持全局变量(bss、data、rodata)、子函数、map、helper。  

SPF/BARE格式文件的好处:  
1. 相比ELF, spf的文件格式非常简单，这使得处理这种格式的runtime代码非常少。  
2. 因为runtime代码非常精简，所以非常容易到处移植  
3. 占用存储空间很少(Flash、RAM需求都很少)，需要的代码段资源也很少，这对资源紧张的嵌入式系统很友好  
4. 简洁的runtime不用频繁更新，大多数的功能升级、演进工作在编译工具中完成  


# runtime说明
| 名称  | 目录 | 说明 |
| --- | --- | --- |
| bare-cmd | mybpf/spf/bare_cmd | 运行BARE文件 |
| bare-interactive | mybpf/spf/runtime/bare_interactive | 以交互模式运行BARE文件 |
| spfcmd | mybpf/spf/runtime/spf_cmd | 运行SPF文件的runtime |
| mini | runtime/mini | 非常小的bare runtime |
| uboot | runtime/uboot | 支持在uboot上运行 |

# 性能测试
测试环境:  
MacBook Pro  芯片：Apple M2 Pro  macOS：14.2.1   

测试方法： 计算Fibonacci，使用time命令计算消耗时间  

测试结果:  
本地指令执行：  
gcc -O3 fibonacci.c  
time ./a.out 10000000000  
执行两次结果  
3.72s user 0.00s system 99% cpu 3.751 total  
3.74s user 0.00s system 98% cpu 3.793 total  

解释执行:  
time ./spfbuilder run fibonacci.o -p 10000000000  
91.65s user 0.01s system 99% cpu 1:32.14 total  
可以看出解释执行性能很低，需要90多秒

JIT执行:  
time ./spfbuilder run fibonacci.o -p 10000000000 -j  
执行两次结果:  
2.89s user 0.00s system 99% cpu 2.908 total  
2.89s user 0.00s system 99% cpu 2.914 total  

编译成SPF格式执行:  
./spfbuilder convert fibonacci.o -j -o fibonacci.spf  
time ./spfbuilder run fibonacci.spf -p 10000000000  
执行两次结果:  
2.89s user 0.00s system 99% cpu 2.911 total  
2.89s user 0.00s system 99% cpu 2.908 total  

编译成BARE格式执行:  
./barebuilder con bare fibonacci.o -o fibonacci.bare  
time ./barebuilder run fibonacci.bare -p 10000000000  
执行两次结果:  
2.89s user 0.00s system 99% cpu 2.910 total  
2.89s user 0.00s system 99% cpu 2.907 total  

# 已测试环境
已经尝试在多种不同环境移植并运行，包括：  
Linux用户态(Centos/Ubuntu)  
Linux内核态  
Macos用户态 (Intel CPU 笔记本、M1笔记本、M2笔记本)  
Windows用户态  
嵌入式(uboot + qemu)  
Openwrt (Newifi2+解释器)  
树莓派 (2B+)  
华为手机 (P20+Termux)  

# 编译spf
cd mybpf  
这里有两个build_xxx.sh文件，分别是不同环境下的编译脚本  

在MACOS机器上  
./build_macos.sh  

在Linux机器上  
./build_linux.sh  

编译结果在: build/out/spf/  

# 用法
## 编译  
  编译为SPF格式  
    spfbuilder convert -j ebpf文件名 -o 输出文件名  

## 执行 
  运行spf格式文件:  
    spfcmd SPF文件名  

## runtime
### bare-cmd
  bare-cmd是以命令行方式执行BARE文件的runtime  
  用法:  bare-cmd file.bare  
  file.bare: 文件名  

### bare_interactive
  bare-interactive是交互模式的 bare runtime  

### spfcmd
  spfcmd是SPF runtime
  用法:  

执行bare_spf:
```
./spfcmd hello_world.o
```

# 使用示例
```
cd mybpf

./build_mac.sh 或者 ./build_linux.sh

cd build/out/spf_runtime
cp ../../../loader/*.bare ./
cp ../../../example/ulc/test/*.o ./

../tool/runbpf con bare test_sub_prog.o -o test_sub_prog.bare
./bare_cmd test_sub_prog.bare

../tool/runbpf con simple test_func_ptr_global.o -o test_func_ptr_global.spf -j
./bare_spf
> load test test_func_ptr_global.spf
> testcmd
> quit

```

# 编写APP 示例
cd example/ulc/test  
创建 hello_world.c, 输入以下内容:  
```
#include "utl/ulc_user.h"

SEC("tcmd/hello_test")
int main()
{
    printf("Hello world!! \n");
    return 0;
}
```

编译成ebpf字节码文件:  
```
clang -O2 -I ../../../h -target bpf -c hello_world.c  -D IN_ULC_USER  
```


