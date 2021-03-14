;
; process 
;

global switch_task_asm ;
global switch_tss_asm ;

; 注意：该函数是 iretd 返回
switch_task_asm:
  mov esp, [esp + 4] ; // 切换 栈空间   
  iretd

;
switch_tss_asm:
  push ebp 
  mov ebp, esp 

  mov eax, [ebp + 8] ; // 取参数 tss selector 

  ltr ax;
  pop ebp
  ret

;  
