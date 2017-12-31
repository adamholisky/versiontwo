#include "kernel.h"

extern void system_interrupt_test( void );
extern void user_mode_test( void );
extern void tss_flush( void );
extern void gdt_flush( uint32_t *gdt_desc );
void *memset(void *s, int c, size_t n);
extern void stack( void );
extern void GDT( void );

struct tss_entry_struct {
   uint32_t prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
   uint32_t esp0;       // The stack pointer to load when we change to kernel mode.
   uint32_t ss0;        // The stack segment to load when we change to kernel mode.
   uint32_t esp1;       // Unused...
   uint32_t ss1;
   uint32_t esp2;
   uint32_t ss2;
   uint32_t cr3;
   uint32_t eip;
   uint32_t eflags;
   uint32_t eax;
   uint32_t ecx;
   uint32_t edx;
   uint32_t ebx;
   uint32_t esp;
   uint32_t ebp;
   uint32_t esi;
   uint32_t edi;
   uint32_t es;         // The value to load into ES when we change to kernel mode.
   uint32_t cs;         // The value to load into CS when we change to kernel mode.
   uint32_t ss;         // The value to load into SS when we change to kernel mode.
   uint32_t ds;         // The value to load into DS when we change to kernel mode.
   uint32_t fs;         // The value to load into FS when we change to kernel mode.
   uint32_t gs;         // The value to load into GS when we change to kernel mode.
   uint32_t ldt;        // Unused...
   uint16_t trap;
   uint16_t iomap_base;
} __attribute__((packed));

struct tss_entry_struct tss_entry;

struct gdt_desc {
   uint16_t		limit;               // The upper 16 bits of all selector limits.
   uint32_t 	base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed));

struct gdt_entry {
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed));

void store_gdt_desc( struct gdt_desc *location );

struct gdt_entry_bits {
	unsigned int limit_low:16;
	unsigned int base_low : 24;
     //attribute byte split into bitfields
	unsigned int accessed :1;
	unsigned int read_write :1; //readable for code, writable for data
	unsigned int conforming_expand_down :1; //conforming for code, expand down for data
	unsigned int code :1; //1 for code, 0 for data
	unsigned int always_1 :1; //should be 1 for everything but TSS and LDT
	unsigned int DPL :2; //priviledge level
	unsigned int present :1;
     //and now into granularity
	unsigned int limit_high :4;
	unsigned int available :1;
	unsigned int always_0 :1; //should always be 0
	unsigned int big :1; //32bit opcodes for code, uint32_t stack for data
	unsigned int gran :1; //1 to use 4k page addressing, 0 for byte addressing
	unsigned int base_high :8;
} __attribute__((packed));

/* Agenda:

	1. done - terminal, printf, debug_f, serial
	2. done - interrupts
	3. done - move kernel to upper half
	4. done - basic system call via interrupt
	5. done - paging
	6. done - user mode
	7. load binary into user mode, run it
	8. pre-emptive multi tasking in user mode
*/

void kernel_main( void ) {
	struct gdt_desc GDTD;
	struct gdt_entry * gdt_e;
	uint16_t gdt_size = sizeof( struct gdt_entry );

	term_setup();
	initalize_interrupts();
	serial_setup();

	initalize_memory();
	initalize_paging();
 
	printf( "VersionTwo\n" );

	debug_f( "Hello, serial port...\n\nNice to see you!\n" );

	store_gdt_desc( &GDTD );
	debug_f( "tssflush: 0x%08X\n", tss_flush );
	debug_f( "main: 0x%08X\n", kernel_main );
	debug_f( "stack: 0x%08X\n", stack );
	debug_f( "gdt size: %d\n", gdt_size );
	debug_f( "base: 0x%08X     limit: 0x%08X\n", GDTD.base, GDTD.limit );

	struct gdt_entry_bits * gdt_entries = (struct gdt_entry_bits *) GDT;

	struct gdt_entry_bits *code = (gdt_entries + 3);
	struct gdt_entry_bits *data = (gdt_entries + 4);
	//I assume your ring 0 segments are in gdt[1] and gdt[2] (0 is null segment)
	//code=(void*)&gdt[3]; //gdt is a static array of gdt_entry_bits or equivalent
	//data=(void*)&gdt[4];
	code->limit_low=0xFFFF;
	code->base_low=0;
	code->accessed=0;
	code->read_write=1; //make it readable for code segments
	code->conforming_expand_down=0; //don't worry about this.. 
	code->code=1; //this is to signal its a code segment
	code->always_1=1;
	code->DPL=3; //set it to ring 3
	code->present=1;
	code->limit_high=0xF;
	code->available=1;
	code->always_0=0;
	code->big=1; //signal it's 32 bits
	code->gran=1; //use 4k page addressing
	code->base_high=0;
	*data=*code; //copy it all over, cause most of it is the same
	data->code=0; //signal it's not code; so it's data.

	struct gdt_entry_bits *g = (gdt_entries + 5);
	uint32_t base = (uint32_t) &tss_entry;
	uint32_t limit = sizeof(tss_entry);
	
	// Now, add our TSS descriptor's address to the GDT.
	g->limit_low=limit&0xFFFF;
	g->base_low=base&0xFFFFFF; //isolate bottom 24 bits
	g->accessed=1; //This indicates it's a TSS and not a LDT. This is a changed meaning
	g->read_write=0; //This indicates if the TSS is busy or not. 0 for not busy
	g->conforming_expand_down=0; //always 0 for TSS
	g->code=1; //For TSS this is 1 for 32bit usage, or 0 for 16bit.
	g->always_1=0; //indicate it is a TSS
	g->DPL=3; //same meaning
	g->present=1; //same meaning
	g->limit_high=(limit&0xF0000)>>16; //isolate top nibble
	g->available=0;
	g->always_0=0; //same thing
	g->big=0; //should leave zero according to manuals. No effect
	g->gran=0; //so that our computed GDT limit is in bytes, not pages
	g->base_high=(base&0xFF000000)>>24; //isolate top byte.

	memset( &tss_entry, 0, sizeof( tss_entry ) );

	tss_entry.ss0 = 0x10;
	tss_entry.esp0 = stack;
	//tss_entry.eax = 0xBABA;
	//tss_entry.cs = 0x0b;
   	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x23;

	gdt_flush( (uint32_t)&GDTD );

	debug_f( "\n\nagain!\n\n" );

	store_gdt_desc( &GDTD );
	debug_f( "gdt size: %d\n", gdt_size );
	debug_f( "base: 0x%08X     limit: 0x%08X\n", GDTD.base, GDTD.limit );

	gdt_e = ( GDTD.base );

	for( int x = 0; x < 6; x++ ) {
		//e = ( GDTD.base + (gdt_size * x) );
		debug_f( "location: 0x%08X    gran: 0x%04X   access:0x%04X\n", (gdt_e + x), gdt_e[ x ].granularity, gdt_e[ x ].access );
	}
	
	debug_f( "loding tr\n" );
	
	tss_flush();

	//system_interrupt_test();

	debug_f( "go umt tr\n" );

	user_mode_test();


	

	while ( 1 ) { ; }
}

/*
static void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}
*/

void store_gdt_desc( struct gdt_desc *location ) {
	__asm__ __volatile__("sgdt %0" : : "m"(*location) : "memory");
}

void *memset(void *s, int c, size_t n)
{
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}