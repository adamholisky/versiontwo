global tss_flush
global gdt_flush
global jump_to_user_mode

tss_flush:
    mov     ax, 0x2B     
    ltr     ax
    ret

gdt_flush:
   mov      eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt     [eax]         ; Load the new GDT pointer

   mov      ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
   mov      ds, ax        ; Load all data segment selectors
   mov      es, ax
   mov      fs, ax
   mov      gs, ax
   mov      ss, ax
   jmp      0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
   ret

jump_to_user_mode:
    mov    ax, 0x23
    mov    ds, ax
    mov    es, ax 
    mov    fs, ax 
    mov    gs, ax 
 
    mov    eax, esp
    push   0x23            ;user data segment 
    push   eax             ;push current stack
    pushf
    pop    eax
    or     eax, 0x200
    push   eax
    push   0x1B            ;user code segment
    mov    eax, 0          ;clear registers before jumping
    mov    ebx, 0
    mov    ecx, 0
    mov    edx, 0
    push   0x00000000
    iret