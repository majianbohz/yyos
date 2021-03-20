//

typedef
struct {
   int  gs_user_mode_spot;
   int  fs_user_mode_spot;
   int  es_user_mode_spot;
   int  ds_user_mode_spot;

   // pushad / popad 部分
   int  edi_user_mode_spot;
   int  esi_user_mode_spot;
   int  ebp_user_mode_spot;
   int  esp_old;             // pushad 指令执行前的 栈位置
   int  ebx_user_mode_spot;
   int  edx_user_mode_spot;
   int  ecx_user_mode_spot;
   int  eax_user_mode_spot;

   // 硬件自动部分
   int  eip_user_mode_spot;
   int  cs_user_mode_spot;
   int  eflags_user_mode_spot;
   int  esp_user_mode_spot;
   int  ss_user_mode_spot;
}__attribute__((packed))  StackFrame_Spot;

typedef
struct {
   // 用于填写 TSS
   int  ss_r0;  // kernel_mode 
   int  esp_r0; // kernel_mode

   int  id;
   char name[20];

   // 进程创建时的值
   short cs_origin; // user_mode:
   int eip_origin;  // user_mode:
   int eflags_origin; // user_mode: IOPL  IF
   short ss_origin; // user_mode:
   int esp_origin;  // user_mode:

   short ds_origin; // user_mode:
   short es_origin; // user_mode:
   short fs_origin; // user_mode:
   short gs_origin; // user_mode:
} __attribute__((packed))  PCB;

typedef 
struct {
//offset 	0 - 15 	16 - 31
int  	LINK 	;// 16 -31 reserved
int 	ESP0  ;
int 	SS0 	;// 16 -31 reserved
int 	ESP1  ;
int 	SS1 	;// 16 -31 reserved
int 	ESP2  ;
int 	SS2 	;// 16 -31 reserved
int 	CR3   ;
int 	EIP   ;
int 	EFLAGS;
int 	EAX;
int 	ECX;
int 	EDX;
int 	EBX;
int 	ESP;
int 	EBP;
int 	ESI;
int 	EDI;
int 	ES 	;// 16 -31 reserved
int 	CS 	;// 16 -31 reserved
int 	SS 	;// 16 -31 reserved
int 	DS 	;// 16 -31 reserved
int 	FS 	;// 16 -31 reserved
int 	GS 	;// 16 -31 reserved
int 	LDTR 	;// 16 -31 reserved
short trap;   ;// reserved 
short	iopboffset ;// IOPB offset
} __attribute__((packed)) TSS;

