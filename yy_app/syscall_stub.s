
;
; void syscall(int code, void* pin, int insize, void* pout, int outsize);
; 4-bytes boundary

global syscall ;

syscall:
  int 0x80 ;
  ret
  
