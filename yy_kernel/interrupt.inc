; usage: macro_ISR_2 interruptNo, kernelDSselector
;        1 interruptNo: dd
;        2 kernelDSselector: dw

%macro  macro_ISR_2 2
global isr%1_raw ;
isr%1_raw:
  nop ; 第一个指令 bochs 设置断点无效，故加一条 nop 指令
isr%1:
  pushad
  push ds
  push es
  push fs
  push gs

  mov ax, %2
  mov ds, ax
  mov es, ax
  mov fs, ax

  mov eax, %1
  push eax   ; 32 bit
  call c_isr
  add esp, 4

  pop gs
  pop fs
  pop es
  pop ds
  popad

  iretd
%endmacro


; usage: macro_ISR_3 interruptNo, kernelDSselector, portNo
;        1 interruptNo: dd
;        2 kernelDSselector: dw
;        3 portNo: db

%macro  macro_ISR_3 3
global isr%1_raw ;
isr%1_raw:
  nop ; 第一个指令 bochs 设置断点无效，故加一条 nop 指令
isr%1:  ; 在这里 bochs 设置断点有效
  pushad
  push ds
  push es
  push fs
  push gs

  mov ax, %2
  mov ds, ax
  mov es, ax
  mov fs, ax

  mov eax, %1
  push eax   ; 32 bit
  call c_isr
  add esp, 4

  mov al, 020h
  out %3, al  ; 020h port No

  pop gs
  pop fs
  pop es
  pop ds
  popad

  iretd
%endmacro
