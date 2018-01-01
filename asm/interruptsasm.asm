align 4

global system_interrupt_test

; extern call_from_asm_test
extern interrupt_default_handler

section .text

global sys_break
sys_break:
     mov eax, 0x1000
     int 0x30
     ret

system_interrupt_test:
	mov eax, 0x8845
    int 0x30
	ret

; Interrupts
; Revised for macro usage
%macro interrupt_handler 1
global interrupt_%1
interrupt_%1:
    pushad
        push ds
        push es
        push fs
        push gs
            push esp
                mov eax, 0x10
                mov ds, eax
                mov es, eax
                mov eax, 0x2
                push eax
                    mov eax, %1
                    push eax
                        cld
                        call interrupt_default_handler
                    pop eax
                pop eax
            pop esp
        pop gs
        pop fs
        pop es
        pop ds
     popad
     iret
%endmacro

interrupt_handler 0x20
interrupt_handler 0x21
interrupt_handler 0x22
interrupt_handler 0x23
interrupt_handler 0x24
interrupt_handler 0x25
interrupt_handler 0x26
interrupt_handler 0x27
interrupt_handler 0x28
interrupt_handler 0x29
interrupt_handler 0x2A
interrupt_handler 0x2B
interrupt_handler 0x2C
interrupt_handler 0x2D
interrupt_handler 0x2E
interrupt_handler 0x2F
interrupt_handler 0x30

; Exceptions 
; Revised for macro usage

%macro exception_handler 1
global interrupt_%1
interrupt_%1:
    pushad
        push ds
        push es
        push fs
        push gs
            push esp
                mov eax, 0x10
                mov ds, eax
                mov es, eax
                mov eax, 0x1
                push eax
                    mov eax, %1
                    push eax
                        cld
                        call interrupt_default_handler
                    pop eax
                pop eax
            pop esp
        pop gs
        pop fs
        pop es
        pop ds
     popad
     iret
%endmacro

exception_handler 0
exception_handler 1
exception_handler 2
exception_handler 3
exception_handler 4
exception_handler 5
exception_handler 6
exception_handler 7
exception_handler 8
exception_handler 9
exception_handler 10
exception_handler 11
exception_handler 12
exception_handler 13
exception_handler 16
exception_handler 17
exception_handler 18
exception_handler 19

extern interrupt_handler_14
global interrupt_14
interrupt_14:
     pusha
        push ds
        push es
        push fs
        push gs
            mov eax, 0x10    ; Data segment
            mov ds, eax
            mov es, eax
            cld
            mov eax, cr2
            push eax
                call interrupt_handler_14    ; Page Fault #PF
            pop eax
        pop gs
        pop fs
        pop es
        pop ds
     popa
     iret
