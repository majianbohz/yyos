
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
TARGET  = masterboot bootloader kernel

# All Phony Targets
.PHONY : default clean_target clean_obj kernel 


default : clean  masterboot bootloader kernel

clean : 
	rm -f $(TARGET) *.o

masterboot : yy_masterboot/*.s
	$(ASM) $(ASMFLAGS_BIN) -o $@ yy_masterboot/*.s
	dd conv=notrunc if=masterboot of=c.img count=1
  
bootloader : yy_bootloader/*.s yy_bootloader/*.c
	rm -f *.o
	$(ASM) $(ASMFLAGS_ELF) -o bootloader.o yy_bootloader/bootloader.s
	$(CC)  $(CCFLAGS)  -o loadkernel.o yy_bootloader/loadkernel.c
	$(LD)  $(LDFLAGS_BOOTLOADER)  -o $@.elf *.o
	objcopy -O binary bootloader.elf bootloader
	dd conv=notrunc if=bootloader of=c.img seek=1
  
kernel : yy_kernel/*.s yy_kernel/*.c
	rm -f *.o
	$(ASM) $(ASMFLAGS_ELF) -o switchdescriptortable.o yy_kernel/switchdescriptortable.s
	$(ASM) $(ASMFLAGS_ELF) -o interrupt.o yy_kernel/interrupt.s
	$(ASM) $(ASMFLAGS_ELF) -o print.o common/print.s
	$(CC)  $(CCFLAGS)  -o kernel.o yy_kernel/kernel.c
	$(CC)  $(CCFLAGS)  -o descriptor.o yy_kernel/descriptor.c
	$(CC)  $(CCFLAGS)  -o convert.o common/convert.c
	$(LD)  $(LDFLAGS_KERNEL)  -o $@.elf *.o
	objcopy -O binary kernel.elf kernel
	cp -f kernel cimg/

