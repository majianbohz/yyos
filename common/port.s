
global in_port_byte ;
global out_port_byte ;

;
in_port_byte:
  push ebp
  mov ebp, esp
 
  mov dx, [ebp + 8] ; dx => io port 
  in al, dx
  mov edi, [ebp + 12]
  mov [edi], al

  mov esp, ebp
  pop ebp
  ret

;
out_port_byte:
  push ebp
  mov ebp, esp
  
  mov dx, [ebp + 8]  ; dx => io port
  mov al, [ebp + 12]
  out dx, al

  mov esp, ebp
  pop ebp
  ret

