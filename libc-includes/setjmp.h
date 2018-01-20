/**
 * @file
 * @brief setjmp/longjmp for the x86 architecture
 *
 * @date 01.12.10
 * @author Nikolay Korotky
 */

#ifndef X86_SETJMP_H_
#define X86_SETJMP_H_

#ifndef __ASSEMBLER__

#define __JMPBUF_LEN  9

typedef int __jmp_buf[__JMPBUF_LEN];
#define jmp_buf __jmp_buf

extern int setjmp( jmp_buf env );
extern void longjmp( jmp_buf env, int val );

#endif /* __ASSEMBLER__ */

#define SETJMP_EBP_INDEX 3
#define SETJMP_ESP_INDEX 4




#endif /* X86_SETJMP_H_ */