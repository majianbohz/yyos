// yyos kernel

extern void printstr(char * msg); 
extern void setprintline(int lineNo);
extern void init_gdt();

void _start() {
    init_gdt();

    setprintline(3);
    printstr("enter yyos kernel...");

    while (1) {}
}


