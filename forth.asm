global _start
%include "util.asm"
%include "macro.inc"

%define pc r15
%define w r14
%define rstack r13

section .text

%include "lib.inc"
%include "words.inc"

state: dq STATE_INTERPRET

section .bss
word_buffer:    resb 1024
resq 1023              
rstack_start: resq 1   

user_mem: resq 65536   

section .data

last_word: dq _lw     
dp: dq user_mem       
stack_start:  dq 0     
 
section .text

_start: 
    mov rstack, rstack_start
    mov [stack_start], rsp
    mov pc, forth_init

next:                  
    mov w, pc
    add pc, 8
    mov w, [w]
    jmp [w]
