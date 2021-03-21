# yyos
yinyu os 

sshubuntu

bochs -q   // bochs 自带调试器 调试
bochsgdb -q -f bochsrc.gdb   // bochs 带的 gdb server 连接 外部的 gdb 调试


#
# use c.img by loop device
#
losetup  -f --show --partscan c.img
fdisk /dev/loop?
mount /dev/loop?p?  /mydir


#
bochs 

bochsgdb -f bochsrc.gdb

#
gdb 
set disassembly-flavor intel
target remote localhost:1234
layout split
layout reg

