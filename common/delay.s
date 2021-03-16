;
global delay_asm ;

delay_asm:
   mov ecx, [esp + 4]
.delay_asm_loop   
   dec ecx
   jnz .delay_asm_loop
   ret

