section .code

execute_testing:
    mov edx, 0x0
    mov eax, 0x8845
umt_loop:
    int 0x30
    inc edx
    cmp edx, 0x04
    jne umt_loop
do_nothing_now:
    jmp do_nothing_now
