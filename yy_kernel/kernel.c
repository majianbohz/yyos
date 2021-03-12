// yyos kernel

extern void printstr(char * msg); 
extern void printc(char  mychar); 
extern void setprintline(int lineNo);
extern void convertInt2Str(int, char*);
extern void init_gdt();
extern void init_idt();
extern void in_port_byte(int port, char *pval);
extern void out_port_byte(int port, char val);

// kernel entry 
void _start() {
  init_gdt();
  init_idt();

  setprintline(3);
  printstr("enter yyos kernel...");

  while (1) {}
}


void c_isr(int irqNo) {
  char buf[10];
  convertInt2Str(irqNo, buf);
  printstr(buf);
 
  if (33 == irqNo) { 
    char val;
    in_port_byte(0x60, &val);
    printc(val);
  }
}

