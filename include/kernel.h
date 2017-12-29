#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#define byte unsigned char
#define word unsigned short
#define dword unsigned int

#include "interrupts.h"
#include "serial.h"
#include "term.h"
#include "base.h"

/* Outputs a byte to the specified hardware port */
static inline void outportb( uint32_t port, uint8_t value)
{
    __asm__ __volatile__ ("outb %%al,%%dx"::"d" (port), "a" (value));
}

/* Outputs a word to a port */
static inline void outportw( uint32_t port, uint32_t value)
{
    __asm__ __volatile__ ("outw %%ax,%%dx"::"d" (port), "a" (value));
}

/* gets a byte from a port */
static inline uint8_t inportb( uint32_t port)
{
    uint8_t value;
    __asm__ __volatile__ ("inb %w1,%b0" : "=a"(value) : "d"(port));
    return value;
}
