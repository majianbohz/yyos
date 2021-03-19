;
; process 
;

global switch_task_asm ;
global switch_tss_asm ;

; 注意：该函数是 iretd 返回
switch_task_asm:
  mov ebp, esp
  mov esp, [ebp + 8] ; // 使用当前SS段, 切换到目标任务的SS段 到 PCB Entry
  mov  ss, [ebp + 4] ; // 使用当前SS段, 切换到目标任务的SS段

  pop gs ;
  pop fs ;
  pop es ;
  pop ds ;
  popad  ;

  ; esp point to tcb.eip_origin 
  iretd

;
switch_tss_asm:
  push ebp 
  mov ebp, esp 

  mov eax, [ebp + 8] ; // 取参数 tss selector 

  ltr ax;
  
  mov esp, ebp
  pop ebp
  ret

;  
