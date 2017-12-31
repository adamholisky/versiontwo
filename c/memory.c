#include "kernel.h"

extern void load_kernel_managed_paging( uint32_t * page_dir );

void * mem_top;

bool allow_mem_debug = false;

uint32_t page_directory[1024] __attribute__((aligned(4096)));

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

	page_directory[ 0 ] = 0x10000087; // loaded binary file from linker, mapped to 0
	page_directory[ kernel_memory_base >> 22 ] = 0x00000087; // Kernel 
	page_directory[ 769 ] = 0x00400087; // Second kernel page can go to physical D... because why not
	page_directory[ 770 ] = 0x00800087;
	page_directory[ 771 ] = 0x00C00087;
	//page_directory[ vga_memory_base >> 22 ] = multiboot_info->framebuffer_addr | 0x00000083; // VGA

	load_kernel_managed_paging( page_directory );
}