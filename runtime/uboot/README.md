# 准备代码
1. 下载uboot对应版本代码:
```
wget https://ftp.denx.de/pub/u-boot/u-boot-2022.10.tar.bz2
tar xjf u-boot-2022.10.tar.bz2
```

2. 将```u-boot-2022.10```中的几个文件拷贝到下载的代码对应位置
```
cp -r mybpf/runtime/uboot/u-boot-2022.10/* ./u-boot-2022.10/
```

# 下载编译器  

到 ```https://developer.arm.com/downloads/-/gnu-a```网站下载arm64编译器，比如下载```gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu.tar.xz```  
将编译器解压，此处我们解压到了```~/cc/```目录  
```
sudo mkdir /cc/
sudo tar xf gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu.tar.xz -C /cc/
```

# 编译
```
export PATH=/cc/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin:$PATH
make qemu_arm64_defconfig
make -j CROSS_COMPILE=aarch64-none-linux-gnu-
```

# tftp
在本地开启tftp服务，准备下载BAREE文件和SPF文件到uboot

# 运行uboot
```
qemu-system-aarch64 -m 512 -machine virt -cpu cortex-a53 -smp 1 -bios u-boot.bin -nographic
```
# 加载SPF runtime和 SPF APP
在uboot命令行下执行：  
```
setenv serverip 192.168.64.8   #192.168.64.8是tftp服务的IP地址，需要根据情况修改自己tftp服务的iP
tftp 0x40200000 spf_loader.arm64.bare
load_loader 0x40200000

tftp 0x40200000 fibonacci.spf
load_spf test1 0x40200000

test_spf 100000

unload_spf test1
```
