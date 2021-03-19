/*
  process manager
*/

#include "process.h"
#include "descriptor.h"

extern void  switch_task_asm(void*);
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
const short selector_task_system_stack_kernel = (14 << 3) | RPL_R0; // 0x70

const short selector_task_system2_code = (15 << 3) | RPL_R1; // 0x79
const short selector_task_system2_data = (16 << 3) | RPL_R1; // 0x81
const short selector_task_system2_stack_kernel = (17 << 3) | RPL_R0; // 0x88

const short selector_task_user1_code = (18 << 3) | RPL_R3;  //0x93
const short selector_task_user1_data = (19 << 3) | RPL_R3;  //0x9b
const short selector_task_user1_stack_kernel = (20 << 3) | RPL_R0; // 0xa0

const short selector_task_user2_code = (21 << 3) | RPL_R3;  //0xab
const short selector_task_user2_data = (22 << 3) | RPL_R3;  //0xb3
const short selector_task_user2_stack_kernel = (23 << 3) | RPL_R0; // 0xb8

void switch_usertask(int processId) {
  PCB *ppcb = &process_control_block[processId];

  // TSS 
  tss.SS0 = ppcb->ss_r0;
  tss.ESP0 = ppcb->esp_r0;
  switch_tss_asm(selector_tss); 

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

  switch_task_asm(pStackFrame); // 参数为 iretd 指令执行前, 需要设置的栈位置
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
        ppcb->ss_origin = selector_task_system_data;
        ppcb->esp_origin = 0x1ffffff; // offset 16M

        ppcb->ds_origin = selector_task_system_data;
        ppcb->es_origin = selector_task_system_data;
        ppcb->fs_origin = selector_task_system_data;
        ppcb->gs_origin = selector_display_seg;
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
        ppcb->ss_origin = selector_task_system2_data;
        ppcb->esp_origin = 0x2ffffff; // offset 16M

        ppcb->ds_origin = selector_task_system2_data;
        ppcb->es_origin = selector_task_system2_data;
        ppcb->fs_origin = selector_task_system2_data;
        ppcb->gs_origin = selector_display_seg;
    }

}
