;

global memcpy_asm ;

memcpy_asm:
   mov ecx, [esp + 12]
   mov esi, [esp + 8]
   mov edi, [esp + 4]
   
   rep movsb 
   ret

