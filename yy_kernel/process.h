//

typedef
struct {
   int  gs;
   int  fs;
   int  es;
   int  ds;
   int  edi;
   int  esi;
   int  ebp; 
   int  esp_old; // pushad 命令执行前的 栈位置
   int  ebx;
   int  edx;
   int  ecx;
   int  eax;

   int  retaddr;

   int  eip_origin;
   int  cs_origin;
   int  eflags_origin;
   int  esp_origin;
   int  ss_origin;

} __attribute__((packed))  TCB;

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

