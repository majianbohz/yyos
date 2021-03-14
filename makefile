
# Makefile for yyos

# Assembly language compiler
ASM      =  nasm
ASMFLAGS_BIN =  -g -f bin 
ASMFLAGS_ELF = -g -f elf32 

# c language compiler
CC       =  gcc
CCFLAGS  =  -g -march=i386 -m32 -ffreestanding -fno-pie  -c 

# Linker
LD       = ld
LDFLAGS_BOOTLOADER  = -melf_i386 -N -Ttext 0x8000 -Map mapbootloader
LDFLAGS_KERNEL  = -melf_i386 -N -Ttext 0x100000 -Map mapkernel 


# Target 
TARGET  = masterboot bootloader.elf  kernel.elf

# All Phony Targets
.PHONY : default clean 


default : clean  masterboot bootloader.elf  kernel.elf

clean : 
	rm -f $(TARGET) *.o

masterboot : yy_masterboot/*.s
	$(ASM) $(ASMFLAGS_BIN) -o $@ yy_masterboot/*.s
	dd conv=notrunc if=masterboot of=c.img count=1
  
bootloader.elf : yy_bootloader/*.s yy_bootloader/*.c  common/*.s common/*.c
	rm -f *.o
	$(ASM) $(ASMFLAGS_ELF) -o bootloader.o yy_bootloader/bootloader.s
	$(CC)  $(CCFLAGS)  -o loadkernel.o yy_bootloader/loadkernel.c
	$(LD)  $(LDFLAGS_BOOTLOADER)  -o $@  bootloader.o loadkernel.o  # bootloader.o must put first, affect binary file out
	objcopy -O binary $@  bootloader
	dd conv=notrunc if=bootloader of=c.img seek=1
  
kernel.elf : yy_kernel/*.s yy_kernel/*.c  common/*.s common/*.c
	rm -f *.o
	$(ASM) $(ASMFLAGS_ELF) -o descriptortable_manage.o yy_kernel/descriptortable_manage.s
	$(ASM) $(ASMFLAGS_ELF) -o interrupt.o yy_kernel/interrupt.s
	$(ASM) $(ASMFLAGS_ELF) -o print.o common/print.s
	$(ASM) $(ASMFLAGS_ELF) -o port.o common/port.s
	$(ASM) $(ASMFLAGS_ELF) -o process_manage.o yy_kernel/process_manage.s
	$(CC)  $(CCFLAGS)  -o kernel.o yy_kernel/kernel.c
	$(CC)  $(CCFLAGS)  -o descriptor.o yy_kernel/descriptor.c
	$(CC)  $(CCFLAGS)  -o process.o yy_kernel/process.c
	$(CC)  $(CCFLAGS)  -o convert.o common/convert.c
	$(LD)  $(LDFLAGS_KERNEL)  -o $@  kernel.o  print.o descriptor.o descriptortable_manage.o interrupt.o convert.o port.o process.o process_manage.o # kernel.o must put first, affect binary file out
	objcopy -O binary $@  kernel
	cp -f kernel cimg/
	ls -l  cimg/
