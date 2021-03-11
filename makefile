
# Makefile for yyos

# Assembly language compiler
ASM      =  nasm
ASMFLAGS =  

# c language compiler
CC       =  gcc
CCFLAGS  = 

# Linker
LD       = ld
LDFLAGS  = 


# Target 
TARGET  = masterboot bootloader kernel

# All Phony Targets
.PHONY : default clean kernel 


default : 

clean : 
	rm -f $(TARGET)  *.o
  
masterboot : yy_masterboot/*.s
	$(ASM) $(ASMFLAGS) -O $@ @<
  
bootloader : yy_bootloader/*.s yy_bootloader/*.c
	$(ASM) $(ASMFLAGS) -O bootloader.o yy_bootloader/bootloader.s
	$(CC)  $(CCFLAGS)  -O loadkernel.o yy_bootloader/loadkernel.c
	$(LD)  $(LDFLAGS)  -O $@ *.o
  
kernel : yy_kernel/*.s yy_kernel/*.c
	$(ASM) $(ASMFLAGS) -O bootloader.o yy_bootloader/bootloader.s
	$(CC)  $(CCFLAGS)  -O loadkernel.o yy_bootloader/loadkernel.c
	$(LD)  $(LDFLAGS)  -O $@ *.o

