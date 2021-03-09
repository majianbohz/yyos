; 
; protected mode tools functions
;

;
; print string
;
printstr:
  push ebp
  mov ebp, esp
  
  mov  esi, [ebp + 8] ; get string address
  mov  edi, [cur_disp_pos] ;
  mov  ah, 0x2f

printstr_loop:
  mov  al, [esi]
  cmp  al, 0
  je lable_printstr_end
  
  mov [gs:edi], ax
  add edi, 2
  inc esi 
  jmp printstr_loop

lable_printstr_end:
  mov [cur_disp_pos], edi  

  mov esp, ebp
  pop ebp
  ret

; print ONE char 
printc:
  push ebp
  mov  ebp, esp

  mov edi, [cur_disp_pos] ;
  mov ah, 0x2f
  mov al, [ebp + 8] ;
  mov [gs:edi], ax
  add edi, 2

  mov [cur_disp_pos], edi  

  mov esp, ebp
  pop ebp
  ret

;;

cur_disp_pos dd 0 
