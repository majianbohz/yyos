// yyos kernel

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

int curProcId = 0;
int keystrokeCnt = 0;

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

void procKeyboardInt() {
    char val;
    in_port_byte(0x60, &val);
    printc(val);
    
    keystrokeCnt++;
    if (0 == keystrokeCnt%2) {
      curProcId++; curProcId = curProcId%2;
      switch_usertask(curProcId);
    }
}
 
