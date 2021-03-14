init8259:
   mov al, 011h
   out 020h, al
   out 0a0h, al
   nop
   nop
   nop
   mov al, 020h
   out 021h, al
   mov al, 028h
   out 0a1h, al
   nop
   nop
   nop
   mov al, 004h
   out 021h, al
   mov al, 002h
   out 0a1h, al
   nop
   nop
   nop
   mov al, 001h
   out 021h, al
   out 0a1h, al  
   nop
   nop
   nop
   
   ;mov al, 11111000b
   mov al, 11111111b
   out 021h, al
   mov al, 11111111b
   out 0a1h, al
   nop
   nop
   nop

   ret

