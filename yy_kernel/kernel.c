// yyos kernel

extern void printstr(char * msg); 
extern void setprintline(int lineNo);
extern voi convertInt2Str(int, char*);
extern void init_gdt();
extern void init_idt();

void _start() {
    init_gdt();
	init_idt();

    setprintline(3);
    printstr("enter yyos kernel...");

    while (1) {}
}


void c_isr(int irqNo) {
    char buf[10];
	convertInt2Str(irqNo, buf)
	printstr(buf);
}

