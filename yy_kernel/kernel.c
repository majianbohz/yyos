// yyos kernel

extern void printstr(char * msg); 
extern void setprintline(int lineNo);

void _start() {
    setprintline(3);
    printstr("enter yyos kernel...");

    while (1) {}
}


