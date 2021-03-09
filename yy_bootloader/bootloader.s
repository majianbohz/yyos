; bootloader 1.0 - bootloader code         
; Author: majianbo  Date: 2021-02-19
; build script: nasm -f bin bootload.s

global _start
global pRawKernelBuffer

extern loadKernelFromExt2

;org 08000h    ; ld -Ttext 0x8000 

; =============================
; =========== 16 bit ==========
; =============================
bits 16
section .text
_start:       
  mov ax, cs
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, 07c00h

  mov dword [gdtPtr +2], label_GDT

  lgdt [gdtPtr]

  cli

  mov eax, cr0
  or  eax, 1 
  mov cr0, eax

  ; Great JUMP to Protect Mode !!!!!!!
  jmp dword selector_kernel : label_seg_code32
  

; ===================================
; ============ 32 bit ===============
; ===================================
bits 32
section .text
label_seg_code32:
   mov ax, selector_kernel_data
   mov ds, ax
   mov es, ax
   mov ss, ax
   mov fs, ax
   mov esp, 0ffffffh  ; 16M 

   mov ax, selector_video
   mov gs, ax

   push enter_protect_mode_tip   
   call printstr
   add esp, 4

   cli

   lidt [idt_info]

   call init8259
   
   int 0x3

  sti

  ; push dword 0a000h
  ; push dword 0
  ; push dword 2
  ; call read_ata_sector
  ; add  esp, 12    ; pop parameters

   call loadKernelFromExt2

  ; jump to kernel !!
   jmp selector_kernel:Kernel_Entry

;ISR_OFF  equ $ - $$    ; get offset in current section
isr0:
   push esi
   push edi

   mov al, 020h
   out 020h, al
 
   in al, 060h  
   push eax
   call printc
   add esp, 4

   pop edi
   pop esi
   iretd


%include "./common/descriptor.inc" ; 
%include "./common/print.s"
%include "./common/a8259.s"
%include "./common/ata.s"

seg_code32_len equ $ - label_seg_code32

; gdt entry index from 0 
[SECTION .data]
label_GDT: Descriptor 0,0,0  ; #0
label_desc_VIDEO: Descriptor 0B8000h, 07fffh, Seg_Data_Attr_4G ; #1 
label_desc_KERNEL: Descriptor 0, 0fffffh, Seg_Code_Attr_4G ; #2
label_desc_KERNEL_Data: Descriptor 0, 0fffffh, Seg_Data_Attr_4G ; #3 

gdtLen  equ $ - label_GDT
gdtPtr  dw gdtLen ;
        dd 0

selector_video        equ label_desc_VIDEO - label_GDT
selector_kernel       equ label_desc_KERNEL - label_GDT
selector_kernel_data  equ label_desc_KERNEL_Data - label_GDT


idt_start:
%rep 255
      dw isr0      ; dw ISR_OFF cs base addr =>0
      dw 0x0010
      db 0x00
      db 10001110b
      dw 0x0000
%endrep
idt_end:

idt_info:
    dw idt_end - idt_start - 1
    dd idt_start

enter_protect_mode_tip db "enter protect mode..." ;
pRawKernelBuffer dd   0100000h
Kernel_Entry     equ  0100000h

