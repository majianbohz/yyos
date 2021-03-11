;
; 
;

extern c_isr ;

; 
isr0:
   push esi
   push edi

   push 0   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

;   
isr1:
   push esi
   push edi

   push 1   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

;   
isr2:
   push esi
   push edi

   push 2   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
   
;
isr3:
   push esi
   push edi

   push 3   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
   
