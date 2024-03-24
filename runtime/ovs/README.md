# 环境
下载OVS代码，v2.14.0版本  
```
git clone https://github.com/openvswitch/ovs.git
cd ovs
git checkout v2.14.0
```
将```mybpf/runtime/ovs/lib/```目录下的文件拷贝到 ovs的lib目录:  
```
cp -r /MYBPF_PATH/runtime/ovs/lib/* /OVS_PATH/ovs/lib/
```
修改 ovs/lib/automake.mk，增加如下几行：
```
lib_libopenvswitch_la_SOURCES = \
+  lib/mybpf_main.c \
+  lib/mybpf_bare.c \
+  lib/bpf_helper_utl.c \
   lib/aes128.c \
```

# 触发执行示例
在需要执行bpf的位置，增加对mybpf调用即可，示例如下：
```
...
#include "utl/mybpf_main.h"
...
MYBPF_PARAM_S p = {0};

/* p.p[0] - p.p[4] 是传递给bpf的5个参数 */
p.p[0] = argc;
p.p[1] = (long)argv;

/* p.bpf_ret为bpf的返回值 */
MYBPF_Notify(MYBPF_HP_TCMD, &p);
...
```

# 编译
到ovs目录下编译：
```
./boot.sh
./configure
make
sudo make install
```


