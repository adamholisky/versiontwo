align 4

global system_interrupt_test

global int22
global int23
global int24
global int25
global int26
global int27
global int28
global int29
global int2A
global int2B
global int2C
global int2D
global int2E
global int2F

extern int_22
extern int_23
extern int_24
extern int_25
extern int_26
extern int_27
extern int_28
extern int_29
extern int_29
extern int_2A
extern int_2B
extern int_2D
extern int_2E
extern int_2F

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


[global int20]
int20:
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax, 0x10    ; Data segment
               mov ds,eax
               mov es,eax
               mov eax, 0x1001
               push eax
                    cld
                    call interrupt_default_handler   ; Test Interrupt Handler
               pop eax
          pop gs
          pop fs
          pop es
          pop ds
     popa
     iret

[global int21]
int21:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call interrupt_default_handler
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int22:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call int_22
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int23:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call int_23
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int24:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call int_24
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int25:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call int_25
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int26:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call int_26
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int27:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call int_27
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int28:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call int_28
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int29:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call int_29
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int2A:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call int_2A
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int2B:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call interrupt_default_handler
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int2C:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call interrupt_default_handler
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int2D:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call interrupt_default_handler
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int2E:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call interrupt_default_handler
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

int2F:
     cli
     pusha
          push ds
          push es
          push fs
          push gs
               mov eax,0x10    ; Data segment
               mov ds,eax
               mov es,eax
               cld
               call interrupt_default_handler
          pop gs
          pop fs
          pop es
          pop ds
     popa
     sti
     iret

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
                mov eax, 0x0
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
