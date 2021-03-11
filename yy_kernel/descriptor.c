#include "descriptor.h"

// 全局描述符表长度
#define GDT_LENGTH 8

// 全局描述符表定义
DESCRIPTOR_ENTRY gdt[GDT_LENGTH];

// GDTR
GDT_PTR gdt_ptr;

#define LDT_LENGTH 256
IDTDescr idt[LDT_LENGTH];

// IDTR
IDT_PTR idt_ptr;

extern unsigned int  isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr32, isr33, isr34, isr35, isr36, isr37, isr38, isr39, isr40, isr41, isr42, isr43, isr44, isr45, isr46, isr47, isrx; 

// 声明内核栈地址
extern unsigned int  stack;

extern void switch_gdt(void* pgdtptr);
extern void switch_idt(void* pgdtptr);


void set_descriptor(int index, unsigned int  base, unsigned int limit, unsigned char attr, unsigned char gran);
void set_idt_descriptor(int index, unsigned int offset, unsigned short selector, unsigned char type_attr); 

// 初始化全局描述符表
void init_gdt()
{
	// 全局描述符表界限 e.g. 从 0 开始，所以总长要 - 1
	gdt_ptr.limit = sizeof(DESCRIPTOR_ENTRY) * GDT_LENGTH - 1;
	gdt_ptr.base = (unsigned int)&gdt;

	// 采用 Intel 平坦模型
	set_descriptor(0, 0, 0, 0, 0);             	// 按照 Intel 文档要求，第一个描述符必须全 0
	set_descriptor(1, 0, 0xFFFFF, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 内核指令段
	set_descriptor(2, 0, 0xFFFFF, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 内核数据段
	set_descriptor(3, 0, 0xFFFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 1#用户进程指令段
	set_descriptor(4, 0, 0xFFFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 1#用户进程数据段
	set_descriptor(5, 0, 0xFFFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 2#用户进程指令段
	set_descriptor(6, 0, 0xFFFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 2#用户进程数据段
	
    set_descriptor(7, 0xB8000, 0x3E80, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_BYTE); 	// 

	// 加载全局描述符表地址到 GPTR 寄存器
    switch_gdt(&gdt_ptr);
}

void init_idt() 
{
  for (int i=20; i<=31; i++) {
    set_idt_descriptor(i, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  }

  for (int i=48; i<LDT_LENGTH; i++) {
    set_idt_descriptor(i, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  }

    set_idt_descriptor(0, isr0, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(1, isr1, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(2, isr2, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(3, isr3, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(4, isr4, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(5, isr5, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(6, isr6, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(7, isr7, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(8, isr8, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(9, isr9, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(10, isr10, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(11, isr11, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(12, isr12, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(13, isr13, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(14, isr14, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(15, isr15, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(16, isr16, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(17, isr17, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(18, isr18, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(19, isr19, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);

    set_idt_descriptor(32, isr32, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(33, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(34, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(35, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(36, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(37, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(38, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(39, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(40, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(41, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(42, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(43, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(44, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(45, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(46, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
    set_idt_descriptor(47, isrx, 0x8, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);

  idt_ptr.limit = sizeof(IDTDescr) * LDT_LENGTH - 1;
  idt_ptr.base = (unsigned int)&idt;
  switch_idt(&idt_ptr);
}

void set_descriptor(int index, unsigned int  base, unsigned int limit, unsigned char attr, unsigned char gran)
{
	gdt[index].base_low     = (base & 0xFFFF);
	gdt[index].base_middle  = (base >> 16) & 0xFF;
	gdt[index].base_high    = (base >> 24) & 0xFF;

	gdt[index].limit_low    = (limit & 0xFFFF);
	gdt[index].limit_hight  = (limit >> 16) & 0x0F;

	gdt[index].attr_low     = attr | 0x80;  //0x80 -> P 段存在标记
	gdt[index].attr_high    = gran | 0x4;   // 0x4 -> D/B 32位   
}

void set_idt_descriptor(int index, unsigned int offset, unsigned short selector, unsigned char type_attr) 
{
	idt[index].offset_1 = offset & 0x0000ffff;
	idt[index].offset_2 = offset>>16 & 0x0000ffff;
	idt[index].selector = selector;
	idt[index].zero = 0;
	idt[index].type_attr = type_attr | 0X80;  //0x80 -> P 段存在标记
}

