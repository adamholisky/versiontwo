#include "kernel.h"

void * mem_top;
struct tss_entry_struct tss_entry;
bool allow_mem_debug = false;

uint32_t page_directory[1024] __attribute__((aligned(4096)));

extern void jump_to_user_mode( void );

/**
 * @brief      Sets up the start of the memory.
 */
void initalize_memory( void ) {
	mem_top = (void *)0xC0080000;
}

void enable_mem_debug( void ) {
	allow_mem_debug = true;
}

/**
 * @brief      Sets up paging for everyone using a new page directory.
 */
void initalize_paging( void ) {
	unsigned int i;
	//multiboot_info_t * multiboot_info = get_multiboot_header();

	for( i = 0; i < 1024; i++ ) {
	    page_directory[i] = 0x00000000;
	}

	page_directory[ 0 ] = 0x00001087; // loaded binary file from linker, mapped to 0
	page_directory[ kernel_memory_base >> 22 ] = 0x00000083; // Kernel 
	page_directory[ 769 ] = 0x00400083; // Second kernel page can go to physical D... because why not
	page_directory[ 770 ] = 0x00800083;
	page_directory[ 771 ] = 0x00C00083;
	//page_directory[ vga_memory_base >> 22 ] = multiboot_info->framebuffer_addr | 0x00000083; // VGA

	load_kernel_managed_paging( page_directory );
}

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

void test_user_mode_app( void ) {
	uint8_t code[] = { 0xba, 0x00, 0x00, 0x00, 0x00, 0xB8, 0x45, 0x88, 0x00, 0x00, 0xCD, 0x30, 0x42, 0x83, 0xFA, 0x05, 0x75, 0xF8, 0xEB, 0xFE };
	uint8_t *mem = (uint8_t *)0;

	for( int i = 0; i < 20; i++ ) {
		mem[i] = *(code + i);
		debug_f( "0x%02X ", *(mem + i ) );
	}

	debug_f( "\n\n" );

	jump_to_user_mode();
}

void initalize_user_mode( void ) {
	struct gdt_entry 		* gdt_entries = (struct gdt_entry *) GDT;
	struct gdt_entry 		* user_code_segment = (gdt_entries + 3);
	struct gdt_entry 		* user_data_segment = (gdt_entries + 4);
	struct gdt_entry 		* tss_segment = (gdt_entries + 5);
	uint32_t 				base = (uint32_t) &tss_entry;
	uint32_t 				limit = sizeof(tss_entry);

	user_code_segment->limit_low	= 0xFFFF;
	user_code_segment->base_low		= 0;
	user_code_segment->accessed		= 0;
	user_code_segment->read_write	= 1; //make it readable for code segments
	user_code_segment->conforming_expand_down	= 0; //don't worry about this.. 
	user_code_segment->code			= 1; //this is to signal its a code segment
	user_code_segment->always_1		= 1;
	user_code_segment->DPL			= 3; //set it to ring 3
	user_code_segment->present		= 1;
	user_code_segment->limit_high	= 0xF;
	user_code_segment->available	= 1;
	user_code_segment->always_0		= 0;
	user_code_segment->big			= 1; //signal it's 32 bits
	user_code_segment->gran			= 1; //use 4k page addressing
	user_code_segment->base_high	= 0;

	*user_data_segment = *user_code_segment; //copy it all over, cause most of it is the same
	user_data_segment->code			= 0; //signal it's not code; so it's data.

	tss_segment->limit_low			= limit&0xFFFF;
	tss_segment->base_low			= base&0xFFFFFF; //isolate bottom 24 bits
	tss_segment->accessed			= 1; //This indicates it's a TSS and not a LDT. This is a changed meaning
	tss_segment->read_write			= 0; //This indicates if the TSS is busy or not. 0 for not busy
	tss_segment->conforming_expand_down	= 0; //always 0 for TSS
	tss_segment->code				= 1; //For TSS this is 1 for 32bit usage, or 0 for 16bit.
	tss_segment->always_1			= 0; //indicate it is a TSS
	tss_segment->DPL				= 3; //same meaning
	tss_segment->present			= 1; //same meaning
	tss_segment->limit_high			= (limit&0xF0000)>>16; //isolate top nibble
	tss_segment->available			= 0;
	tss_segment->always_0			= 0; //same thing
	tss_segment->big				= 0; //should leave zero according to manuals. No effect
	tss_segment->gran				= 0; //so that our computed GDT limit is in bytes, not pages
	tss_segment->base_high			= (base&0xFF000000)>>24; //isolate top byte.

	memset( &tss_entry, 0, sizeof( tss_entry ) );
	tss_entry.ss0 					= 0x10;
	tss_entry.esp0 					= stack;
   	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x23;

	tss_flush();
}