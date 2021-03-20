
;
; void syscall(int code, void* pin, int insize, void* pout, int outsize);
; 4-bytes boundary

global syscall ;

syscall:
  
  int 0x80 ;
  
;  mov ebp, esp
;  mov esi, [ebp + 16] ; pout
;  mov ecx, [ebp + 20] ; outsize
;  shr ecx, 2  ; 4-bytes  32bits
  
;.label_syscall:
;  pop eax
;  mov [esi], eax
;  shl esi, 2
;  loop .label_syscall
  
  ret
  
