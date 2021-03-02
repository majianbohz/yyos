; bootloader 1.0 - bootloader code         
; Author: majianbo  Date: 2021-02-19
; build script: nasm -f bin bootload.s

%include "descriptor.inc" ; 

org 08000h
    jmp label_start16

[SECTION .GDT]
label_GDT: Descriptor 0,0,0
label_desc_VIDEO: Descriptor 0B8000h, 07fffh, Seg_Data_Attr_1M ; 
label_desc_CODE32: Descriptor 0, seg_code32_len, Seg_Code_Attr_1M ;
label_desc_KERNEL: Descriptor 0100000h, 0fffffh, Seg_Code_Attr_1M ;
label_desc_KERNEL_Data: Descriptor 01000000h, 0fffffh, Seg_Data_Attr_1M ; 

gdtLen  equ $ - label_GDT
gdtPtr  dw gdtLen ;
        dd 0

selector_video        equ label_desc_VIDEO - label_GDT
selector_code32       equ label_desc_CODE32 - label_GDT
selector_kernel       equ label_desc_KERNEL - label_GDT
selector_kernel_data  equ label_desc_KERNEL_Data - label_GDT


[SECTION .IDT]
idt_start:
%rep 255
      dw ISR_OFF
      dw 0x0010
      db 0x00
      db 10001110b
      dw 0x0000
%endrep
idt_end:

idt_info:
    dw idt_end - idt_start - 1
    dd idt_start

bits 16
section .text16
label_start16:
  mov ax, cs
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, 07c00h

  ; 初始化 32位 代码段描述符
  xor eax, eax
  mov ax, cs
  shl eax, 4
  add eax, label_seg_code32
  mov ebx, label_desc_CODE32
  call fn_set_seg_base  ; focus stack space
  
  ;
  mov dword [gdtPtr +2], label_GDT

  lgdt [gdtPtr]

  cli

  mov eax, cr0
  or  eax, 1 
  mov cr0, eax

  ; Great JUMP to Protect Mode !!!!!!!
  jmp dword selector_code32 : 0
  
; parameter: eax <- seg_base
;            ebx <- descriptor_base 
fn_set_seg_base:
  mov word [ebx + 2], ax
  shr eax, 16
  mov byte [ebx + 4], al
  mov byte [ebx + 7], ah
  ret

; ===================================
; ============ 32 bit ===============
; ===================================
bits 32
section .text32
label_seg_code32:
   mov ax, selector_kernel_data
   mov ss, ax
   mov esp, 0fffffh

   mov ax, selector_video
   mov gs, ax
   
   mov edi, (80 * 10 + 0) *2 ;
   mov ah, 0Ch
   mov al, 'P'
   mov [gs:edi], ax

   cli

   lidt [idt_info]

   call init8259
   
   int 0x3

   sti

   mov esi, ata_read_param
   mov edi, 0a000h
   call read_ata_sector

  mov ax, selector_kernel_data
  mov es, ax
  mov edi, 0
  mov esi, ata_read_param
  mov dword [esi+1], 65    
  call read_ata_sector
 
  jmp $

ISR_OFF  equ $ - $$    ; get offset in current section
isr0:
   push esi
   push edi

   mov edi, (80 *10 +10) *2
   inc byte [gs:edi]

   mov al, 020h
   out 020h, al
 
   in al, 060h  
   mov byte [gs:edi], al 

   pop edi
   pop esi
   iretd

%include "a8259.inc"

%include "ata.inc"

ata_read_param:
  db  2
  dd  0  

seg_code32_len equ $ - label_seg_code32

