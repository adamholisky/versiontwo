#include "kernel.h"
#include <stdio.h>
#include <string.h>

kernel_symbols * ksym = NULL; 
uint32_t num_syms = 0;

void initalize_symbol_table( multiboot_info_t * multiboot_info ) {
    debug_f( "ELF Num: %d\n", multiboot_info->u.elf_sec.num );
	debug_f( "ELF Size: %d\n", multiboot_info->u.elf_sec.size );
	debug_f( "ELF Addr: 0x%08x\n", multiboot_info->u.elf_sec.addr );
	debug_f( "ELF Indx: %d\n", multiboot_info->u.elf_sec.shndx );

	uint32_t * addr = 0xC0000000 + multiboot_info->u.elf_sec.addr;

	debug_f( "addr: 0x%08X\n", addr );
	debug_f( "sieof elfheader: %d\n", sizeof( Elf32_Shdr ) );

	Elf32_Shdr * current_elf_section = (Elf32_Shdr *)addr;
	uint32_t * sym_table = NULL;
	char * string_table = NULL;
	Elf32_Shdr * elf_strings_header = NULL;

	for( int x = 0; x <  multiboot_info->u.elf_sec.num; x++ ) {
		debug_f( "Section %d at 0x%08X Type: %d\n", x, current_elf_section, current_elf_section->sh_type );

		if( current_elf_section->sh_type == SHT_SYMTAB ) {
			sym_table = 0xC0000000 + current_elf_section->sh_addr;

			//printf( "Found a symbol table header starting at 0x%08X of 0x%X bytes .\n", sym_table, current_elf_section->sh_size );

			num_syms = current_elf_section->sh_size / current_elf_section->sh_entsize;

			//printf( "This table contains %d symbols.\n", num_syms );
		}

		if( current_elf_section->sh_type == SHT_STRTAB ) {
			if( string_table == NULL ) {
				string_table = 0xC0000000 + current_elf_section->sh_addr;
				elf_strings_header = current_elf_section;

				//printf( "Found a string table header starting at 0x%08X 0x%X bytes.\n", string_table, current_elf_section->sh_size );
			}
		}

		current_elf_section++;
	}
    
    ksym = malloc( sizeof( kernel_symbols ) * num_syms );
	Elf32_Sym * elf_symbols = sym_table;

	for( int y = 0; y < num_syms; y++ ) {
		char * symbol_name = NULL;
		if( elf_symbols[y].st_name != 0 ) { 
			symbol_name = ( string_table + elf_symbols[y].st_name );
            ksym[ y ].entry_addr = elf_symbols[ y ].st_value;
            ksym[ y ].name = symbol_name;
			//debug_f( "A symbol at 0x%08X named %s\n", elf_symbols[y].st_value, symbol_name );
		}
	}
}

kernel_symbols * get_ksym( void ) {
    return ksym;
}

uint32_t get_num_syms( void ) {
    return num_syms;
}