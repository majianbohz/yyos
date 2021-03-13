// yyos kernel

extern void printstr(char * msg); 
extern void printc(char  mychar); 
extern void setprintline(int lineNo);
extern void convertInt2Hex(int, char*);
extern void convertChar2Hex(char myChar, char* str);
extern void init_gdt();
extern void init_idt();
extern void init_tss_descriptor();
extern void in_port_byte(int port, char *pval);
extern void out_port_byte(int port, char val);
extern void entry_task_init();

// kernel entry 
void _start() {
  init_gdt();
  init_idt();

  setprintline(3);
  printstr("enter yyos kernel...");

  // set TSS 
  init_tss_descriptor();

  entry_task_init();

  while (1) {}
}


void c_isr(unsigned int irqNo) {
  char buf[10];
  convertChar2Hex((unsigned char)irqNo, buf);
  printstr(buf);
 
  // 33 keyboard irq#
  if (33 == irqNo) { 
    char val;
    in_port_byte(0x60, &val);
    printc(val);
  }
}

void  task_init() {
   while (1) {
     printc('K');
   }  
}
