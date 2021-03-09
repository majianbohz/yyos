; 
; protected mode tools functions
;
global  printstr      ;
global  printc        ;
global  setprintline  ; 
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
  mov al, [ebp + 8] ; get Print Char Code
  mov [gs:edi], ax
  add edi, 2

  mov [cur_disp_pos], edi  

  mov esp, ebp
  pop ebp
  ret

; set display line No.
setprintline:
    push ebp
    mov  ebp, esp
 
    mov eax, [ebp + 8] ; get line No parameter
    mov bl, 80*2  ; 80 * 25  
    mul bl; 
    mov [cur_disp_pos], eax
 
    mov esp, ebp
    pop ebp
    ret

;;

cur_disp_pos dd 0 
