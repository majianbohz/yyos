extern void printstr(char * msg);
extern void printc(char  mychar);
extern void setprintline(int lineNo);
extern void convertInt2Hex(int, char*);
extern void convertChar2Hex(char myChar, char* str);
extern void procKeyboardInt();
extern void procTimerInt();
extern void procSyscall();

void c_isr(unsigned int irqNo) {
  char buf[10];
  buf[0] = ' ';
  buf[1] = 'I';
  convertChar2Hex((unsigned char)irqNo, buf+2);
  printstr(buf);

  // 33 keyboard irq#
  if (32 == irqNo) {
    procTimerInt();
  }

  // 33 keyboard irq#
  if (33 == irqNo) {
    procKeyboardInt();
  }
    
  if (128 == irqNo) {
    procSyscall();
  }
}
