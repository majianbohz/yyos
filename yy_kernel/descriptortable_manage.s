; -------------------------------------------------
; 启用新GDT 
; -------------------------------------------------

global switch_gdt
global switch_idt

extern selector_kernel_code;
extern selector_kernel_data;
extern selector_display_seg;


switch_gdt:
    cli

	mov eax, [esp + 4]  ; 参数gdt_ptr 存入 eax 寄存器
	lgdt [eax]        ; 加载到 GDTR 

	mov ax, selector_kernel_data    ; 加载我们的数据段描述符
	mov ds, ax      ; 更新所有可以更新的段寄存器
	mov es, ax
	mov fs, ax
	mov ss, ax

    mov ax, selector_display_seg
    mov gs, ax
    
	; NOTE 
	jmp selector_kernel_code:.switch_gdt_stub   ; 启用新的GDT -> 远跳转 
                                ; 远跳目的是清空流水线并串行化处理器
.switch_gdt_stub:
    sti			; 开启中断
	ret

switch_idt:
    cli

	mov eax, [esp+4]  ; 参数 idt_ptr 存入 eax 寄存器
	lidt [eax]        ; 加载到 IDTR 	

    sti
    ret
