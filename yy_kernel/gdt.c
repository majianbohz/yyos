#include "gdt.h"

// 全局描述符表长度
#define GDT_LENGTH 8

// 全局描述符表定义
DESCRIPTOR_ENTRY gdt[GDT_LENGTH];

// GDTR
GDT_PTR gdt_ptr;


// 声明内核栈地址
extern unsigned int  stack;

extern void switch_gdt(void* pgdtptr);


void set_descriptor(int index, unsigned int  base, unsigned int limit, unsigned char attr, unsigned char gran);

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

void set_descriptor(int index, unsigned int  base, unsigned int limit, unsigned char attr, unsigned char gran)
{
	gdt[index].base_low     = (base & 0xFFFF);
	gdt[index].base_middle  = (base >> 16) & 0xFF;
	gdt[index].base_high    = (base >> 24) & 0xFF;

	gdt[index].limit_low    = (limit & 0xFFFF);
	gdt[index].limit_hight  = (limit >> 16) & 0x0F;

	gdt[index].attr_low     = attr + 0x80;  //0x80 -> P 段存在标记
	gdt[index].attr_high    = gran + 0x4;   // D/B =1 32位   
}

