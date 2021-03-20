// yyos kernel

#include "process.h"
#include "kernel.h"

extern void printstr(char * msg); 
extern void printc(char  mychar); 
extern void setprintline(int lineNo);
extern void convertInt2Hex(int, char*);
extern void convertChar2Hex(char myChar, char* str);
extern void init_gdt();
extern void init_idt();
extern void init_tr();
extern void init_tss_descriptor();
extern void in_port_byte(int port, char *pval);
extern void out_port_byte(int port, char val);
extern void switch_usertask(int taskId);
extern void create_process(int processId);
extern void delay_asm(int num);
extern void memcpy_asm(void* dest, void* src, int num);
extern int  get_descriptor_base_addr(short selector);
extern void syscall(int code, void* pin, int insize, void* pout, int outsize);

extern PCB  process_control_block[10];

unsigned int curProcId = 0;
unsigned int keystrokeCnt = 0;
unsigned int timerCnt = 0;

// kernel entry 
void _start() {
  init_gdt();
  init_idt();
  
  init_tr();

  setprintline(3);
  printstr("enter yyos kernel");

  create_process(0);
  create_process(1);
  switch_usertask(curProcId);

  while (1) {
    printc('.');
    delay_asm(10000000);
  }
}

// 系统进程  (编译进内核)
void  process_system() {
   char buf[20];
   buf[0] = ' ';
   buf[1] = 'K';
   buf[2] = 'A';
   int i=0;
   while (1) {
     convertInt2Hex(i++, buf+3);
     printstr(buf);
     delay_asm(100000000);
   }  
}

// 系统进程  (编译进内核)
void  process_system2() {
    char buf2[20];
   //syscall(int code, void* pin, int insize, void* pout, int outsize)
    syscall(0x80, &buf2, 20, &buf2, 20);
    
   char buf[20];
   buf[0] = ' ';
   buf[1] = 'K';
   buf[2] = 'B';
   int i=0;
   while (1) {
     convertInt2Hex(i++, buf+3);
     printstr(buf);
     delay_asm(100000000);
   }
}

void procTimerInt() {
    timerCnt++;
    if (0 == timerCnt%200) {
      curProcId++; curProcId = curProcId%2;
      switch_usertask(curProcId);
    }
}

void procKeyboardInt() {
    char val;
    in_port_byte(0x60, &val);
    printc(val);
    
    /*
    keystrokeCnt++;
    if (0 == keystrokeCnt%2) {
      curProcId++; curProcId = curProcId%2;
      switch_usertask(curProcId);
    }
    */
}

// 基于一个前提：内核DS段和SS段的基址相同。
void procSyscall() {
  PCB* ppcb = &process_control_block[curProcId];
  StackFrame_Spot* pSpotStackFrame = (StackFrame_Spot*)(ppcb->esp_r0 - sizeof(StackFrame_Spot));

  int _ss_baseaddr = get_descriptor_base_addr(pSpotStackFrame->ss_user_mode_spot);
  int _esp = pSpotStackFrame->esp_user_mode_spot;
    
  SysCall_Param *psyscall_param = (SysCall_Param*)(_ss_baseaddr + _esp + 4); // 4: 32bit return address value
  //psyscall_param->code;
    
  int retVal = 0x8080;
  memcpy_asm(psyscall_param->pout, &retVal, sizeof(int));
}

