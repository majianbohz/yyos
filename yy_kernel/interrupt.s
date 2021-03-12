;
; 
;
global isr0 ;
global isr1 ;
global isr2 ;
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18 
global isr19
global isr32
global isr33
global isr34
global isr35
global isr36
global isr37
global isr38
global isr39
global isr40
global isr41
global isr42
global isr43
global isr44
global isr45
global isr46
global isr47
global isrx  

extern c_isr ;
extern printc ;

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

   mov al, 020h
   out 020h, al  ; 020h port No
   
   pop edi
   pop esi
   iretd

;   
isr33:
   push esi
   push edi

   mov eax, 33
   push eax   ; 32 bit
   call c_isr
   add esp, 4

   mov al, 020h
   out 020h, al  ; 020h port No

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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
   mov al, 020h
   out 0a0h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
   mov al, 020h
   out 0a0h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
   mov al, 020h
   out 0a0h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
   mov al, 020h
   out 0a0h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
   mov al, 020h
   out 0a0h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
   mov al, 020h
   out 0a0h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
   mov al, 020h
   out 0a0h, al  ; 020h port No
   
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
   
   mov al, 020h
   out 020h, al  ; 020h port No
   
   mov al, 020h
   out 0a0h, al  ; 020h port No
   
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

   mov al, 020h
   out 020h, al  ; 020h port No
   out 0a0h, al
   
   pop edi
   pop esi
   iretd
 

