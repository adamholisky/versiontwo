global user_mode_test
global tss_flush
global gdt_flush

user_mode_test:
     mov ax,0x23
     mov ds,ax
     mov es,ax 
     mov fs,ax 
     mov gs,ax ;we don't need to worry about SS. it's handled by iret
 
     mov eax,esp
     push 0x23 ;user data segment with bottom 2 bits set for ring 3
     push eax ;push our current stack just for the heck of it
     pushf
     pop eax
     or eax, 0x200
     push eax
     push 0x1B; ;user code segment with bottom 2 bits set for ring 3
     push execute_testing ;may need to remove the _ for this to work right 
     iret

execute_testing:
    mov edx, 0x0
    mov eax, 0x8845
    
umt_loop:
    int 0x30
    inc edx
    cmp edx, 0x10
    jne umt_loop
do_nothing_now:
    jmp do_nothing_now


tss_flush:
    mov ax, 0x2B      ; Load the index of our TSS structure - The index is
                     ; 0x28, as it is the 5th selector and each is 8 bytes
                     ; long, but we set the bottom two bits (making 0x2B)
                     ; so that it has an RPL of 3, not zero.
    ltr ax            ; Load 0x2B into the task state register.
    ret


gdt_flush:
   mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer

   mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax        ; Load all data segment selectors
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
   ret
