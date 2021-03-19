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

extern void isr255_raw();
extern void isr0_raw();
extern void isr1_raw();
extern void isr2_raw();
extern void isr3_raw();
extern void isr4_raw();
extern void isr5_raw();
extern void isr6_raw();
extern void isr7_raw();
extern void isr8_raw();
extern void isr9_raw();
extern void isr10_raw();
extern void isr11_raw();
extern void isr12_raw();
extern void isr13_raw();
extern void isr14_raw();
extern void isr15_raw();
extern void isr16_raw();
extern void isr17_raw();
extern void isr18_raw();
extern void isr19_raw();
extern void isr32_raw();
extern void isr33_raw();
extern void isr34_raw();
extern void isr35_raw();
extern void isr36_raw();
extern void isr37_raw();
extern void isr38_raw();
extern void isr39_raw();
extern void isr40_raw();
extern void isr41_raw();
extern void isr42_raw();
extern void isr43_raw();
extern void isr44_raw();
extern void isr45_raw();
extern void isr46_raw();
extern void isr47_raw();

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
  
  // 内核进程（DS段和内核DS段重合）: Ring 1 code & data segments
  // 栈段的基址 要与 数据段的基址 保持一致，用于保证 C 语言中的 全局变量 和 局部变量 的指针都基于相同的基址
  set_descriptor(12, 0, 0xFFFFF, DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 0#进程指令段 4G
  set_descriptor(13, 0, 0xFFFFF, DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 0#进程数据段 4G
  //  0#进程用户态栈段 4K
  //  段起始地址：0x1FFE000 + 0xFFF + 0x1 = 0x1FFF000  栈顶0x1FFFFFF
  set_descriptor(14, 0, 0x1FFE,  DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_STACK_RW, GRAN_4K);
  // 0#系统进程内核栈段 向下扩展(使用这个目的是为了段基址DS/SS保持一致，这样C中的全局变量和局部变量偏址就一致了)
  // 段起始地址：0xFFD000 + 0xFFF + 0x1 = 0xFFE000  4K大小 栈顶 0xFFEFFF
  set_descriptor(15, 0, 0xFFD,  DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_STACK_RW, GRAN_4K);

  set_descriptor(16, 0, 0xFFFFF, DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 1#系统进程指令段 16M
  set_descriptor(17, 0, 0xFFFFF, DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 1#系统进程数据段 16M
  //  0#进程用户态栈段 4K
  //  段起始地址：0x2FFE000 + 0xFFF + 0x1 = 0x2FFF000  栈顶0x1FFFFFF
  set_descriptor(18, 0, 0x2FFE,  DPL_R1 | SEG_FLAG_DATA_CODE | SEG_TYPE_STACK_RW, GRAN_4K);
  set_descriptor(19, 0, 0xFFC,  DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_STACK_RW, GRAN_4K);  // 0#系统进程内核栈段 向下扩展  段起始地址：0xFFC000 + 0xFFF + 0x1 = 0xFFD000

  // 用户任务: Ring 3 code & data segments
  set_descriptor(20, 0x2000000, 0xFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 2#用户进程指令段 16M
  set_descriptor(21, 0x2000000, 0xFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 2#用户进程数据段 16M
  set_descriptor(22, 0xFFC000, 0x1000, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_BYTE); 	// 2#系统进程内核栈段 4K
  
  set_descriptor(24, 0x3000000, 0xFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_CODE_XR, GRAN_4K); 	// 3#用户进程指令段 16M
  set_descriptor(25, 0x3000000, 0xFFF, DPL_R3 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_4K); 	// 3#用户进程数据段 16M
  set_descriptor(26, 0xFFB000, 0x1000, DPL_R0 | SEG_FLAG_DATA_CODE | SEG_TYPE_DATA_RW, GRAN_BYTE); 	// 3#系统进程内核栈段 4K
  
  // 加载全局描述符表地址到 GPTR 寄存器
  switch_gdt(&gdt_ptr);
}

void init_idt() 
{
  for (int i=20; i<=31; i++) {
    set_idt_descriptor(i, &isr255_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  }

  for (int i=48; i<IDT_LENGTH; i++) {
    set_idt_descriptor(i, &isr255_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  }

  set_idt_descriptor(0, &isr0_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(1, &isr1_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(2, &isr2_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(3, &isr3_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(4, &isr4_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(5, &isr5_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(6, &isr6_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(7, &isr7_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(8, &isr8_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(9, &isr9_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(10, &isr10_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(11, &isr11_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(12, &isr12_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(13, &isr13_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(14, &isr14_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(15, &isr15_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(16, &isr16_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(17, &isr17_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(18, &isr18_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(19, &isr19_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);

  set_idt_descriptor(32, &isr32_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(33, &isr33_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(34, &isr34_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(35, &isr35_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(36, &isr36_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(37, &isr37_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(38, &isr38_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(39, &isr39_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(40, &isr40_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(41, &isr41_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(42, &isr42_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(43, &isr43_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(44, &isr44_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(45, &isr45_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(46, &isr46_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);
  set_idt_descriptor(47, &isr47_raw, selector_kernel_code, DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_INT);

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
  DESCRIPTOR* pdesc = &gdt[selector>>3];
  int base = (pdesc->base_high << 24) | (pdesc->base_middle << 16) | pdesc->base_low;
  int limit = (pdesc->limit_hight<<16) | pdesc->limit_low;
  if (0 != (pdesc->attr_high & GRAN_4K))
    limit = limit * 0x1000; //4K

  return  base + limit;  
}

