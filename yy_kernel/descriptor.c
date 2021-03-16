#include "descriptor.h"
#include "process.h"

// 全局描述符表长度
#define GDT_LENGTH 30 // 

// 全局描述符表定义
DESCRIPTOR gdt[GDT_LENGTH];

// GDTR
GDT_PTR gdt_ptr;

#define IDT_LENGTH 256
IDTDescr idt[IDT_LENGTH];

// IDTR
IDT_PTR idt_ptr;

TSS  tss;

extern short selector_kernel_code;

extern void isrx();
extern void isr0();
extern void isr1();
extern void isr2(); 
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6(); 
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11(); 
extern void isr12(); 
extern void isr13(); 
extern void isr14(); 
extern void isr15(); 
extern void isr16(); 
extern void isr17(); 
extern void isr18(); 
extern void isr19();
extern void isr32(); 
extern void isr33(); 
extern void isr34(); 
extern void isr35(); 
extern void isr36(); 
extern void isr37(); 
extern void isr38(); 
extern void isr39();
extern void isr40(); 
extern void isr41(); 
extern void isr42(); 
extern void isr43(); 
extern void isr44(); 
extern void isr45(); 
extern void isr46(); 
extern void isr47(); 

// 声明内核栈地址
extern unsigned int  stack;

extern void switch_gdt(void* pgdtptr);
extern void switch_idt(void* pgdtptr);

// 初始化全局描述符表
void init_gdt()
{
  // 全局描述符表界限 e.g. 从 0 开始，所以总长要 - 1
  gdt_ptr.limit = sizeof(DESCRIPTOR) * GDT_LENGTH - 1;
  gdt_ptr.base = (unsigned int)&gdt;
  
  // 采用 Intel 平坦模型
  set_descriptor(0, 0, 0, 0, 0);             	// 按照 Intel 文档要求，第一个描述符必须全 0
  set_descriptor(1, 0xB8000, 0x3E80, DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_BYTE); 	// 
  set_descriptor(2, (unsigned int)&tss, sizeof(TSS), DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_TSS, GRAN_BYTE); 	// TSS
 
  // 内核: Ring0 code & data segments
  set_descriptor(10, 0, 0xFFFFF, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 内核指令段 4G
  set_descriptor(11, 0, 0xFFFFF, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 内核数据段 4G
  // 内核栈顶 16M
  
  // 内核任务: Ring 1 code & data segments
  set_descriptor(12, 0, 0xFFFFF, DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 0#系统进程指令段 16M
  set_descriptor(13, 0, 0xFFFFF, DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 0#系统进程数据段 16M
  set_descriptor(14, 0xFFE000, 0x1000, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_BYTE); 	// 0#系统进程内核栈段 4K

  set_descriptor(15, 0, 0xFFFFF, DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 1#系统进程指令段 16M
  set_descriptor(16, 0, 0xFFFFF, DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 1#系统进程数据段 16M
  set_descriptor(17, 0xFFD000, 0x1000, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_BYTE); 	// 1#系统进程内核栈段 4K
  
  // 用户任务: Ring 3 code & data segments
  set_descriptor(18, 0x2000000, 0xFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 2#用户进程指令段 16M
  set_descriptor(19, 0x2000000, 0xFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 2#用户进程数据段 16M
  set_descriptor(20, 0xFFC000, 0x1000, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_BYTE); 	// 2#系统进程内核栈段 4K
  
  set_descriptor(21, 0x3000000, 0xFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 3#用户进程指令段 16M
  set_descriptor(22, 0x3000000, 0xFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 3#用户进程数据段 16M
  set_descriptor(23, 0xFFB000, 0x1000, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_BYTE); 	// 3#系统进程内核栈段 4K
  
  // 加载全局描述符表地址到 GPTR 寄存器
  switch_gdt(&gdt_ptr);
}

void init_idt() 
{
  for (int i=20; i<=31; i++) {
    set_idt_descriptor(i, &isrx, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  }

  for (int i=48; i<IDT_LENGTH; i++) {
    set_idt_descriptor(i, &isrx, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  }

  set_idt_descriptor(0, &isr0, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(1, &isr1, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(2, &isr2, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(3, &isr3, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(4, &isr4, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(5, &isr5, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(6, &isr6, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(7, &isr7, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(8, &isr8, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(9, &isr9, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(10, &isr10, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(11, &isr11, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(12, &isr12, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(13, &isr13, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(14, &isr14, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(15, &isr15, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(16, &isr16, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(17, &isr17, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(18, &isr18, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(19, &isr19, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);

  set_idt_descriptor(32, &isr32, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(33, &isr33, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(34, &isr34, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(35, &isr35, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(36, &isr36, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(37, &isr37, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(38, &isr38, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(39, &isr39, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(40, &isr40, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(41, &isr41, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(42, &isr42, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(43, &isr43, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(44, &isr44, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(45, &isr45, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(46, &isr46, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(47, &isr47, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);

  idt_ptr.limit = sizeof(IDTDescr) * IDT_LENGTH - 1;
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

void set_idt_descriptor(int index, void * offset, unsigned short selector, unsigned char type_attr) 
{
  unsigned int base = (unsigned int)offset;
	idt[index].offset_1 = base & 0x0000ffff;
	idt[index].offset_2 = (base >> 16) & 0x0000ffff;
	idt[index].selector = selector;
	idt[index].zero = 0;
	idt[index].type_attr = type_attr | 0X80;  //0x80 -> P 段存在标记
}

// from GDT
int get_descriptor_high_addr(short selector) {
  DESCRIPTOR* pdesc = gdt[selector>>3];
  int base = (pdesc->base_high << 24) | (pdesc->base_mid << 16) | pdesc->base_low;
  int limit = (pdesc->limit_hight<<16) | pdesc->limit_low;
  if (0 == (pdesc->attr_high & GRAN_4K))
    limit = limit * 0x1000; //4K

  return  base + limit;  
}

