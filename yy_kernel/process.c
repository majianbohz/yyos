/*
  process manager
*/

#include "process.h"
#include "descriptor.h"

extern void  switch_task_asm(int ss_r0, void* pStackFrame);
extern void  switch_tss_asm(int tss_selector);
extern void  memcpy_asm(void* dest, void* src, int num);
extern void  process_system();  // 系统进程 -> 系统第一个进程 Ring 1
extern void  process_system2();  // 系统进程 -> 系统第一个进程 Ring 1
extern TSS tss;

PCB  process_control_block[10];

const short selector_display_seg = (1 << 3) | RPL_R0;  //0x8
const short selector_tss = (2 << 3) | RPL_R0;  //0x10

const short selector_kernel_code = (10 << 3) | RPL_R0;  // 0x50
const short selector_kernel_data = (11 << 3) | RPL_R0;  // 0x58

const short selector_task_system_code = (12 << 3) | RPL_R1; // 0x61
const short selector_task_system_data = (13 << 3) | RPL_R1; // 0x69
const short selector_task_system_stack_user = (14 << 3) | RPL_R1; // 0x71
const short selector_task_system_stack_kernel = (15 << 3) | RPL_R0; // 0x78

const short selector_task_system2_code = (16 << 3) | RPL_R1; // 0x81
const short selector_task_system2_data = (17 << 3) | RPL_R1; // 0x89
const short selector_task_system2_stack_user = (18 << 3) | RPL_R1; // 0x91
const short selector_task_system2_stack_kernel = (19 << 3) | RPL_R0; // 0x98

const short selector_task_user1_code = (20 << 3) | RPL_R3;  //0x9a
const short selector_task_user1_data = (21 << 3) | RPL_R3;  //0xa3
const short selector_task_user1_stack_user = (22 << 3) | RPL_R3; // 0xaa
const short selector_task_user1_stack_kernel = (23 << 3) | RPL_R0; // 0xb0

const short selector_task_user2_code = (24 << 3) | RPL_R3;  //
const short selector_task_user2_data = (25 << 3) | RPL_R3;  //
const short selector_task_user2_stack_user = (26 << 3) | RPL_R3; // 0xaa
const short selector_task_user2_stack_kernel = (27 << 3) | RPL_R0; //

void init_tr() {
    switch_tss_asm(selector_tss);
}

// 注意：这个函数是不会返回的 ！！
// 切换到目标进程的内核栈 和 Spot栈帧底
void switch_usertask(int processId) {
  PCB *ppcb = &process_control_block[processId];

  // TSS 
  tss.SS0 = ppcb->ss_r0;
  tss.ESP0 = ppcb->esp_r0;

  void* pStackFrame = (void*)(ppcb->esp_r0 - sizeof(StackFrame_KernelTask));
  switch_task_asm(ppcb->ss_r0, pStackFrame); // 参数为 iretd 指令执行前, 需要设置的栈位置
}

void init_process_kernel_stack(PCB* ppcb) {
    StackFrame_KernelTask  stackFrameKernelTask;
    stackFrameKernelTask.cs_user_mode_spot = ppcb->cs_origin;
    stackFrameKernelTask.eip_user_mode_spot = ppcb->eip_origin;
    stackFrameKernelTask.eflags_user_mode_spot = ppcb->eflags_origin;
    stackFrameKernelTask.ss_user_mode_spot = ppcb->ss_origin;
    stackFrameKernelTask.esp_user_mode_spot = ppcb->esp_origin;

    stackFrameKernelTask.ds_user_mode_spot = ppcb->ds_origin;
    stackFrameKernelTask.es_user_mode_spot = ppcb->ds_origin;
    stackFrameKernelTask.fs_user_mode_spot = ppcb->fs_origin;
    stackFrameKernelTask.gs_user_mode_spot = ppcb->gs_origin;
    
    void* pStackFrame = (void*)(ppcb->esp_r0 - sizeof(StackFrame_KernelTask));
    memcpy_asm(pStackFrame, &stackFrameKernelTask, sizeof(StackFrame_KernelTask));
}

void create_process(int processId) {
    PCB* ppcb = &process_control_block[processId];
    
    if (0 == processId) {
        ppcb->ss_r0 = selector_task_system_stack_kernel;
        ppcb->esp_r0 = 0xFFF000;
        ppcb->id = processId;
        ppcb->name[0]='s';
        ppcb->name[1]='1';
        ppcb->name[2]=0;
        
        ppcb->cs_origin = selector_task_system_code;
        ppcb->eip_origin = (int)&process_system;
        ppcb->eflags_origin = 0x3202; // IOPL  IF
        ppcb->ss_origin = selector_task_system_stack_user;
        ppcb->esp_origin = 0x2000000; // offset 16M

        ppcb->ds_origin = selector_task_system_data;
        ppcb->es_origin = selector_task_system_data;
        ppcb->fs_origin = selector_task_system_data;
        ppcb->gs_origin = selector_display_seg;
        
        init_process_kernel_stack(ppcb);
    }
    else if (1 == processId) {
        ppcb->ss_r0 = selector_task_system2_stack_kernel;
        ppcb->esp_r0 = 0xFFE000;
        ppcb->id = processId;
        ppcb->name[0]='s';
        ppcb->name[1]='2';
        ppcb->name[2]=0;
        
        ppcb->cs_origin = selector_task_system2_code;
        ppcb->eip_origin = (int)&process_system2;
        ppcb->eflags_origin = 0x3202; // IOPL  IF
        ppcb->ss_origin = selector_task_system2_stack_user;
        ppcb->esp_origin = 0x3000000; // offset 16M

        ppcb->ds_origin = selector_task_system2_data;
        ppcb->es_origin = selector_task_system2_data;
        ppcb->fs_origin = selector_task_system2_data;
        ppcb->gs_origin = selector_display_seg;
        
        init_process_kernel_stack(ppcb);
    }
}

