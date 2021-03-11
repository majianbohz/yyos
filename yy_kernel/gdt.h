// 描述符
typedef
struct DESCRIPTOR_ENTRY {
	unsigned short limit_low;     // 段界限   15～0
	unsigned short  base_low;     // 段基地址 15～0
	unsigned char  base_middle;   // 段基地址 23～16
	unsigned char  attr_low;      // 0~3: 段类型  4: S 描述符类型 (0=系统段 1=数据段OR代码段) 5~6：DPL 描述符特权级 7: P  段存在位
	unsigned char  limit_hight:4;  // 段界限 19～16
	unsigned char  attr_high:4;    // 0 : AVL(留给系统软件使用)  1: 0 （固定值） 2: D/B (0=16位段 1=32位段) 3: G (0=byte 1=4kbyte)
	unsigned char  base_high;     // 段基地址 31～24
} __attribute__((packed)) DESCRIPTOR_ENTRY;

// GDTR
typedef
struct GDT_PTR {
	unsigned short limit; 	// 全局描述符表限长
	unsigned int  base; 	// 全局描述符表 32位 基地址
} __attribute__((packed)) GDT_PTR;

// 数据段 代码段
#define SEG_TYPE_DATA_RW 2
#define SEG_TYPE_CODE_XR 10

// 系统段
#define SEG_TYPE_TSS 9
#define SEG_TYPE_INT 14
#define SEG_TYPE_CALLGATE 12
#define SEG_TYPE_LDT  2
#define SEG_TYPE_TASKGATE 5

#define GRAN_BYTE  0
#define GRAN_4K    8

#define SEG_FLAG_SYS 0x0
#define SEG_FLAG_DATA_CODE 0x10

#define DPL_R0 0x0 
#define DPL_R1 0x20
#define DPL_R2 0x40
#define DPL_R3 0x60

#define RPL_R0 0x0 
#define RPL_R1 0x20
#define RPL_R2 0x40
#define RPL_R3 0x60

/*

type_attr

 7                           0
+---+---+---+---+---+---+---+---+
| P |  DPL  | S |    GateType   |
+---+---+---+---+---+---+---+---+
 */
typedef
struct IDTDescr {
	uint16_t offset_1; // offset bits 0..15
	uint16_t selector; // a code segment selector in GDT or LDT
	uint8_t zero;      // unused, set to 0
	uint8_t type_attr; // type and attributes, see below
	uint16_t offset_2; // offset bits 16..31
} __attribute__((packed)) IDTDescr;

// IDTR
typedef
struct IDT_PTR {
	unsigned short limit; 	// 描述符表限长
	unsigned int  base; 	// 描述符表 32位 基地址
} __attribute__((packed)) IDT_PTR;

