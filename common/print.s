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
  je .label_printstr_end
  
  mov [gs:edi], ax
  add edi, 2
  inc esi 
  jmp printstr_loop

.label_printstr_end:
  cmp edi, display_buf_limit
  jl .label_printstr_end2
  mov edi, 0
  
.label_printstr_end2  
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

  cmp edi, display_buf_limit
  jl .label_printc_end
  mov edi, 0
  
.label_printc_end
  mov [cur_disp_pos], edi  

  mov esp, ebp
  pop ebp
  ret

; set display line No.
setprintline:
    push ebp
    mov  ebp, esp
 
    mov eax, [ebp + 8] ; get line No parameter
    mov bl, 80*2  ; 80 * 25   screen one char use 2 char
    mul bl; 

	cmp eax, display_buf_limit
    jl .label_setprintline_end
    mov eax, 0
  
.label_setprintline_end
	mov [cur_disp_pos], eax
 
    mov esp, ebp
    pop ebp
    ret

;;

cur_disp_pos dd 0 
display_buf_limit equ 80*25*2
