# yyos
yinyu os 


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
target remote localhost:1234

set disassmble-flavor intel


