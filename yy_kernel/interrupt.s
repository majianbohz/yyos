;
; 
;
global isr0 ;
global isr1 ;
global isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr32, isr33, isr34, isr35, isr36, isr37, isr38, isr39, isr40, isr41, isr42, isr43, isr44, isr45, isr46, isr47, isrx 

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

   push 3  ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr4:
   push esi
   push edi

   push 4   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr5:
   push esi
   push edi

   push 5   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr6:
   push esi
   push edi

   push 6   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr7:
   push esi
   push edi

   push 7   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr8:
   push esi
   push edi

   push 8   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr9:
   push esi
   push edi

   push 9   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr10:
   push esi
   push edi

   push 10   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr11:
   push esi
   push edi

   push 11   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr12:
   push esi
   push edi

   push 12   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr13:
   push esi
   push edi

   push 13   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr14:
   push esi
   push edi

   push 14   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr15:
   push esi
   push edi

   push 15   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

; 
isr16:
   push esi
   push edi

   push 16   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd


; 
isr17:
   push esi
   push edi

   push 17   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd


; 
isr18:
   push esi
   push edi

   push 18   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd


; 
isr19:
   push esi
   push edi

   push 19   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd



;   
isr32:
   push esi
   push edi

   push 32   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd

;   
isr33:
   push esi
   push edi

   push 33   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
   
;
isr34:
   push esi
   push edi

   push 34   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
   
;
isr35:
   push esi
   push edi

   push 35   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr36:
   push esi
   push edi

   push 36   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr37:
   push esi
   push edi

   push 37   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr38:
   push esi
   push edi

   push 38   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr39:
   push esi
   push edi

   push 39   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr40:
   push esi
   push edi

   push 40   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr41:
   push esi
   push edi

   push 41   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr42:
   push esi
   push edi

   push 42   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr43:
   push esi
   push edi

   push 43   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr44:
   push esi
   push edi

   push 44   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr45:
   push esi
   push edi

   push 45   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr46:
   push esi
   push edi

   push 46   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr47:
   push esi
   push edi

   push 47   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isr48:
   push esi
   push edi

   push 48   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 
;
isrx:
   push esi
   push edi

   push 255   ; 32 bit
   call c_isr
   add esp, 4
   
   pop edi
   pop esi
   iretd
 

