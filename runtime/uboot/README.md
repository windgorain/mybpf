# 环境准备
下载uboot对应版本代码:
```
wget https://ftp.denx.de/pub/u-boot/u-boot-2022.10.tar.bz2
tar xjf u-boot-2022.10.tar.bz2
```

将```u-boot-2022.10```中的几个文件夹拷贝到下载的代码对应位置  

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

# 运行
```
qemu-system-aarch64 -m 512 -machine virt -cpu cortex-a53 -smp 1 -bios u-boot.bin -nographic
```
