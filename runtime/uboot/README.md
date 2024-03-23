# 环境准备
下载uboot对应版本代码:
```
wget https://ftp.denx.de/pub/u-boot/u-boot-2022.10.tar.bz2
tar xjf u-boot-2022.10.tar.bz2
```

将```u-boot-2022.10```中的几个文件夹拷贝到下载的代码对应位置:
```
cp mybpf/runtime/uboot/uboot-2022.10/* ./u-boot-2022.10
```

# 编译
```
export PATH=/cc/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin:$PATH
make qemu_arm64_defconfig
make -j CROSS_COMPILE=aarch64-none-linux-gnu-
```
