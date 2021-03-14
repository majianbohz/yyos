/*
  process manager
*/

#include "process.h"
#include "descriptor.h"

extern void  switch_task_asm(void*);
extern void  switch_tss_asm(int tss_selector);
extern void  task_init();  // init -> first task


TCB  process_control_block[5];

const short selector_tss = (2 << 3) | RPL_R0;  //0x10

const short selector_kernel_code = (10 << 3) | RPL_R0;  // 0x50
const short selector_kernel_data = (11 << 3) | RPL_R0;  // 0x58
const short selector_task_kernel1_code = (12 << 3) | RPL_R0; // 0x60
const short selector_task_kernel1_data = (13 << 3) | RPL_R0; // 0x68
const short selector_task_kernel2_code = (14 << 3) | RPL_R0; // 0x70
const short selector_task_kernel2_data = (15 << 3) | RPL_R0; // 0x78
const short selector_task_user1_code = (16 << 3) | RPL_R3;  //0x83
const short selector_task_user1_data = (17 << 3) | RPL_R3;  //0x8b
const short selector_task_user2_code = (18 << 3) | RPL_R3;  //0x93
const short selector_task_user2_data = (19 << 3) | RPL_R3;  //0x9b

void switch2TaskUser1() {
  tss.SS0 = selector_task_user1_data;
  tss.ESP0 = 0xffffff; //16M 
}

void switch2TaskUser2() {
  tss.SS0 = selector_task_user2_data;
  tss.ESP0 = 0xffffff; //16M 
}

void entry_task_init() {
  tss.SS0 = selector_task_kernel1_data;
  tss.ESP0 = 0xffffff; //16M
  switch_tss_asm(selector_tss) 

  TCB tcb = process_control_block[0];
  tcb.cs_origin = selector_task_kernel1_code;
  tcb.esp_origin = 0xffffff; // offset 16M 
  tcb.eip_origin = (int)&task_init;

  switch_task_asm(&tcb.eip_origin);
}

