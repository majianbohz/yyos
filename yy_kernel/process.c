/*
  process manager
*/

#include "process.h"
#include "descriptor.h"

extern void  task_init();

TCB  process_control_block[5];

TSS  tss;

const short selector_kernel_code = (10 << 3) | RPL_R0;
const short selector_kernel_data = (11 << 3) | RPL_R0;
const short selector_task_kernel1_code = (12 << 3) | RPL_R0;
const short selector_task_kernel1_data = (13 << 3) | RPL_R0;
const short selector_task_kernel2_code = (14 << 3) | RPL_R0;
const short selector_task_kernel2_data = (15 << 3) | RPL_R0;
const short selector_task_user1_code = (16 << 3) | RPL_R3;
const short selector_task_user1_data = (17 << 3) | RPL_R3;
const short selector_task_user2_code = (18 << 3) | RPL_R3;
const short selector_task_user2_data = (19 << 3) | RPL_R3;

void init_tss_descriptor() {
  set_descriptor(2, &tss, sizeof(TSS), DPL_R0 | SEG_FLAG_SYS | SEG_TYPE_TSS, GRAN_BYTE); 	// TSS
}

void switch2TaskInit() {
  tss.SS0 = selector_task_kernel1_data;
  tss.ESP0 = 0xffffff; //16M 
}

void switch2TaskUser1() {
  tss.SS0 = selector_task_user1_data;
  tss.ESP0 = 0xffffff; //16M 
}

void switch2TaskUser2() {
  tss.SS0 = selector_task_user2_data;
  tss.ESP0 = 0xffffff; //16M 
}

void init_proces() {
  TCB tcb = process_control_block[0];
  tcb.cs_origin = selector_task_kernel1_code;
  tcb.esp_origin = 0xffffff; // offset 16M 
  tcb.eip = &task_init;
}

