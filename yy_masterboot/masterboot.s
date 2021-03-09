; masterboot 1.0 - Master bootblock code         
; Author: majianbo  Date: 2021-02-19

; const define
DISP_RAM_SEG  equ 0xB800
LOAD_OFF      equ 0x8000   ; bootloader load memory location
MBR_OFF    equ 0x7c00
SECTOR_NUM equ 20    ; [2~21] 共20扇区 10K data 

org MBR_OFF

bits 16
section .text
  mov ax, cs
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, MBR_OFF

  ; clear screen
  mov ax, DISP_RAM_SEG
  mov ds, ax
  mov ecx, 1998   ; 80*25=2000 character, 2 bytes per character 
loop_disp:
  mov byte [ecx*2], '' ; ds = 0xB800
  mov byte [ecx*2+1], 0x2f  ; hight 4 bit back, low 4 bit front
  loop loop_disp

  mov byte [0], '#'
  mov byte [1], 0x2f

;;; read_sectors_16
;;;
;;; Read sectors from disk in memory using BIOS services
;;;
;;; input:      dl      = drive
;;;             ch      = cylinder[7:0]        
;;;             cl[7:6] = cylinder[9:8]
;;;             dh      = head
;;;             cl[5:0] = sector (1-63)
;;;             es:bx   -> destination
;;;             al      = number of sectors
;;;
;;; output:     cf (0 = success, 1 = failure)

;read sections for next boot & kernel
mov  ax, 0
mov  es, ax
mov  bx, LOAD_OFF ; ES:BX 数据存储缓冲区
mov  dh, 0        ;DH 用来存储起始磁头号
mov  ch, 0        ;CH 用来存储起始柱面号 [7:0]=> cylinder[7:0]
mov  cl, 2        ;CL 用来存储起始扇区号 [5:0]=> sector, [7:6]=> cylinder [9:8] 
; mov  dl, 0        ;dl 驱动器号  bios 会设置启动的驱动器号到DL寄存器中
mov ah, 0x02      ; read sector function code
mov al, SECTOR_NUM ;
int 13h
jc err

success:
mov byte [4], 'S'   ; success
mov byte [5], 0x2f
jmp LOAD_OFF        ; jump to boot loader !!

err:
mov byte [4], 'F'   ; fail
mov byte [5], 0x24
jmp $

; times 510-($-$$) db 0
; db  0x55, 0xAA

