/*
  process manager
*/

#include "process.h"
#include "descriptor.h"

extern void  switch_task_asm(void*);
extern void  switch_tss_asm(int tss_selector);
extern void  memcpy_asm(void* dest, void* src, int num);
extern void  process_system();  // 系统进程 -> 系统第一个进程 Ring 1
extern TSS tss;

TCB  process_control_block[5];

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

void switch2TaskUser1() {
  tss.SS0 = selector_task_user1_data;
  tss.ESP0 = 0xffffff; //16M 
}

void switch2TaskUser2() {
  tss.SS0 = selector_task_user2_data;
  tss.ESP0 = 0xffffff; //16M 
}

void switch_usertask(int taskId) {

 // TCB
  TCB tcb = process_control_block[taskId];
  tcb.ss_r0 = selector_task_system_stack_kernel;
  tcb.esp_r0 = 0xfff; //4k
  tcb.id = taskId;
  tcb.name[0]='s';
  tcb.name[1]=0;

  // TSS 
  tss.SS0 = tcb.ss_r0;
  tss.ESP0 = tcb.esp_r0;
  switch_tss_asm(selector_tss); 

  StackFrame_KernelTask  stackFrameKernelTask;
  stackFrameKernelTask.cs_origin = selector_task_system_code;
  stackFrameKernelTask.eip_origin = (int)&process_system;
  stackFrameKernelTask.eflags_origin = 0x3202; // IOPL  IF  
  stackFrameKernelTask.ss_origin = selector_task_system_data;
  stackFrameKernelTask.esp_origin = 0xffffff; // offset 16M 

  stackFrameKernelTask.ds = selector_task_system_data;
  stackFrameKernelTask.es = selector_task_system_data;
  stackFrameKernelTask.fs = selector_task_system_data;
  stackFrameKernelTask.gs = selector_display_seg; 
  
  int stackTopKernelTask = get_descriptor_high_addr(selector_task_system_stack_kernel);
  void* pStackFrame = (void*)(stackTopKernelTask-sizeof(StackFrame_KernelTask));
  memcpy_asm(pStackFrame, &stackFrameKernelTask, sizeof(StackFrame_KernelTask));

  switch_task_asm(pStackFrame); // 参数为 iretd 指令执行前, 需要设置的栈位置
}

