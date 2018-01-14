#include "kernel.h"
#include <string.h>

struct page_directory_entry {
	uint8_t present 			: 1;
	uint8_t read_write			: 1;
	uint8_t user_supervisor		: 1;
	uint8_t write_through		: 1; 

	uint8_t cache_disabled		: 1;
	uint8_t accessed 			: 1;
	uint8_t always_zero			: 1;
	uint8_t page_size			: 1;

	uint8_t ignored				: 1;
	uint8_t available			: 3;

	uint32_t page_table_addr	: 20;
} __attribute__((packed));

void * mem_top;
struct tss_entry_struct tss_entry;
bool allow_mem_debug = false;
bool malloc_lock = true;

uint32_t current_page = 0xD0000000 >> 22;
uint32_t real_mem_base = 0x00800000;

uint32_t page_directory[1024] __attribute__((aligned(4096)));

extern void jump_to_user_mode( void );

/**
 * @brief      Sets up the start of the memory.
 */
void initalize_memory( void ) {
	initalize_paging();

	mem_top = get_free_page( 1 );
	malloc_lock = false;
}

void * get_free_page( uint16_t num ) {
	void * ret_addr = (void *)(current_page << 22);

	page_directory[ current_page ] = real_mem_base | 0x83;

	debug_f2( "[get_free_page] Allocated Page: %d -- phys_addr: 0x%08X -- virt_addr: 0x%08X\n", current_page, real_mem_base, ret_addr );

	real_mem_base = real_mem_base + 0x00400000;
	current_page++;

	load_kernel_managed_paging( page_directory );

	return ret_addr;
}

void * kmalloc( uint32_t size ) {
	void * ret_val = mem_top;

	mem_top = mem_top + size;

	return ret_val;
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

	page_directory[ 0 ] = 0x00C00087; // loaded binary file from linker, mapped to 0
	page_directory[ kernel_memory_base >> 22 ] = 0x00000083; // Kernel 
	page_directory[ 769 ] = 0x00400083;
	//page_directory[ 770 ] = 0x00600083;
	//page_directory[ 771 ] = 0x00700083;
	//page_directory[ vga_memory_base >> 22 ] = multiboot_info->framebuffer_addr | 0x00000083; // VGA

	load_kernel_managed_paging( page_directory );
}

void mem_tests( void ) {
	uint8_t * mem_a = kmalloc( sizeof( uint8_t ) );
	uint16_t * mem_b = kmalloc( sizeof( uint16_t ) * 1024 );
	uint32_t * mem_c = kmalloc( sizeof( uint32_t ) * 4096 );
	uint8_t * mem_d = kmalloc( sizeof( uint8_t ) );
	uint8_t * mem_e = kmalloc( sizeof( uint8_t ) );

	debug_f2( "=====Memory Tests=====\n" );
	debug_f2( "mem_a (1 byte):     0x%0X8\n", mem_a );
	debug_f2( "mem_b (1024 bytes): 0x%0X8\n", mem_b );
	debug_f2( "mem_c (4096 bytes): 0x%0X8\n", mem_c );
	debug_f2( "mem_d (1 byte):     0x%0X8\n", mem_d );
	debug_f2( "mem_e (1 byte):     0x%0X8\n", mem_e );

	*mem_a = 'A';
	strcpy( (char *)mem_b, "Katherine Holisky" );

	debug_f2( "mem_a:              %c\n", *mem_a );
	debug_f2( "mem_b:              %s\n", mem_b );

	struct page_directory_entry * pt_entry_a = (struct page_directory_entry *)page_directory;
	struct page_directory_entry * pt_entry_b = (struct page_directory_entry *)(page_directory + 768);
	struct page_directory_entry * pt_entry_current = (struct page_directory_entry *)(page_directory + current_page - 1);

	debug_f2( "page_dir 000:  0x%08X\n", page_directory[0] );
	debug_f2( "pt_entry 000:  p:%x r:%x u:%x w:%x d:%x a:%x 0:%x s:%x ig:%x av:%x phy_addr: 0x%08X\n", pt_entry_a->present, pt_entry_a->read_write, pt_entry_a->user_supervisor, pt_entry_a->write_through, pt_entry_a->cache_disabled, pt_entry_a->accessed, pt_entry_a->always_zero, pt_entry_a->page_size, pt_entry_a->ignored, pt_entry_a->available, (pt_entry_a->page_table_addr << 12) );

	debug_f2( "page_dir 768:  0x%08X\n", page_directory[768] );
	debug_f2( "pt_entry 768:  p:%x r:%x u:%x w:%x d:%x a:%x 0:%x s:%x ig:%x av:%x phy_addr: 0x%08X\n", pt_entry_b->present, pt_entry_b->read_write, pt_entry_b->user_supervisor, pt_entry_b->write_through, pt_entry_b->cache_disabled, pt_entry_b->accessed, pt_entry_b->always_zero, pt_entry_b->page_size, pt_entry_b->ignored, pt_entry_b->available, pt_entry_b->page_table_addr << 12 );

	debug_f2( "page_dir %d:  0x%08X\n", current_page - 1 );
	debug_f2( "pt_entry %d:  p:%x r:%x u:%x w:%x d:%x a:%x 0:%x s:%x ig:%x av:%x phy_addr: 0x%08X\n", current_page - 1, pt_entry_current->present, pt_entry_current->read_write, pt_entry_current->user_supervisor, pt_entry_current->write_through, pt_entry_current->cache_disabled, pt_entry_current->accessed, pt_entry_current->always_zero, pt_entry_current->page_size, pt_entry_current->ignored, pt_entry_current->available, pt_entry_current->page_table_addr << 12 );
	debug_f2( "==========\n" );
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

	debug_f( "Jumping to user mode...\n\n" );

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
	tss_entry.esp0 					= (uint32_t)stack;
   	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x23;

	tss_flush();
}
