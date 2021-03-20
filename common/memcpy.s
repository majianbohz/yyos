;

global memcpy_asm ;

; void  memcpy_asm(void* dest, void* src, int num);
memcpy_asm:
   mov ecx, [esp + 12]
   mov esi, [esp + 8]
   mov edi, [esp + 4]
   
   rep movsb 
   ret

