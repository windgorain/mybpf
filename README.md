# 功能
mybpf是一款轻量的ebpf运行时，可以在用户态、嵌入式、内核等多种环境运行。支持多种方式运行ebpf：字节码解释执行、jit成本机指令执行、编译为SPF/BARE文件运行。  

# 特点
mybpf的特点  
  1. 轻量：runtime极度轻量，能以极低的代价到处移植  
  2. 兼容：兼容多种运行环境，包括嵌入式、内核、用户态、跨OS、跨硬件平台  
  3. 灵活：APP开发难度低，部署灵活快速，可以快上快下，可以动态加载、卸载APP  
  4. 高性能：可以编译为本地指令执行  
  5. 节省空间：BARE和SPF格式文件非常小，占用空间小  

# 架构
mybpf主要分为两部分： 编译工具 + runtime。  
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

# 编译
cd mybpf  
这里有两个build_xxx.sh文件，分别是不同环境下的编译脚本  

在MACOS机器上  
./build_macos.sh  

在Linux机器上  
./build_linux.sh  

编译结果在: build/out/spf/  

# 用法
## 转换为SPF格式  
  spfbuilder convert ebpf文件名 -o 输出文件名 -j

## 执行 
  运行ebpf文件:  
    spfcmd 文件名  

## runtime
### bare-cmd
  bare-cmd是以命令行方式执行BARE文件的runtime  
  用法:  bare-cmd file.bare  
  file.bare: 文件名  

### bare_interactive
  bare-interactive是交互模式的 bare runtime  

### spfcmd
  spfcmd是SPF runtime

# 使用示例
```
cd mybpf

./build_mac.sh 或者 ./build_linux.sh

cd build/out/spf
cp -r ../../../loader/* ./
cp ../../../example/*.o ./

# 直接执行ebpf文件
./spfcmd hello_world.o

# 转换为spf文件执行
./spfbuilder convert hello_world.o -o hello_world.spf -j
./spfcmd hello_world.spf

# 转换为bare文件执行
./barebuilder convert hello_world.o -o hello_world.bare
./bare-cmd hello_world.bare
```

# 编写APP 示例
cd example
创建 hello_world.c, 输入以下内容:  
```
#include "utl/ulc_user.h"

SEC("tcmd/hello_test")
int main()
{
    BPF_Print("Hello world!! \n");
    return 0;
}
```

编译成ebpf字节码文件:  
```
clang -O2 -I ../h -target bpf -c hello_world.c  -D IN_ULC_USER  
```


