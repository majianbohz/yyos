;
; 
;
%include "yy_kernel/interrupt.inc" ;

extern c_isr ;
extern selector_kernel_data;

macro_ISR_2 0, [selector_kernel_data]  ;
macro_ISR_2 1, [selector_kernel_data]  ;
macro_ISR_2 2, [selector_kernel_data]  ;
macro_ISR_2 3, [selector_kernel_data]  ;
macro_ISR_2 4, [selector_kernel_data]  ;
macro_ISR_2 5, [selector_kernel_data]  ;
macro_ISR_2 6, [selector_kernel_data]  ;
macro_ISR_2 7, [selector_kernel_data]  ;
macro_ISR_2 8, [selector_kernel_data]  ;
macro_ISR_2 9, [selector_kernel_data]  ;
macro_ISR_2 10, [selector_kernel_data]  ;
macro_ISR_2 11, [selector_kernel_data]  ;
macro_ISR_2 12, [selector_kernel_data]  ;
macro_ISR_2 13, [selector_kernel_data]  ;
macro_ISR_2 14, [selector_kernel_data]  ;
macro_ISR_2 15, [selector_kernel_data]  ;
macro_ISR_2 16, [selector_kernel_data]  ;
macro_ISR_2 17, [selector_kernel_data]  ;
macro_ISR_2 18, [selector_kernel_data]  ;
macro_ISR_2 19, [selector_kernel_data]  ;

macro_ISR_3 32, [selector_kernel_data], 020h  ;
macro_ISR_3 33, [selector_kernel_data], 020h  ;
macro_ISR_3 34, [selector_kernel_data], 020h  ;
macro_ISR_3 35, [selector_kernel_data], 020h  ;
macro_ISR_3 36, [selector_kernel_data], 020h  ;
macro_ISR_3 37, [selector_kernel_data], 020h  ;
macro_ISR_3 38, [selector_kernel_data], 020h  ;
macro_ISR_3 39, [selector_kernel_data], 020h  ;
macro_ISR_3 40, [selector_kernel_data], 0A0h  ;
macro_ISR_3 41, [selector_kernel_data], 0A0h  ;
macro_ISR_3 42, [selector_kernel_data], 0A0h  ;
macro_ISR_3 43, [selector_kernel_data], 0A0h  ;
macro_ISR_3 44, [selector_kernel_data], 0A0h  ;
macro_ISR_3 45, [selector_kernel_data], 0A0h  ;
macro_ISR_3 46, [selector_kernel_data], 0A0h  ;
macro_ISR_3 47, [selector_kernel_data], 0A0h  ;

macro_ISR_2 128, [selector_kernel_data]  ;
macro_ISR_2 255, [selector_kernel_data]  ;

