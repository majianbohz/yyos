;
; process 
;

global switch_task_asm ;

switch_task_asm:
  
  ;push ebp
  mov ebp, esp
 
  mov esp, [ebp + 4] ; 
  iretd

