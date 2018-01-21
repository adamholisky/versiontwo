#include "kernel.h"
#include <stdio.h>

void added_function( void );
char peek_char( char c );
void peek( uint32_t * address );

void kernel_main( unsigned long multiboot_magic, multiboot_info_t * multiboot_info ) {
	initalize_multiboot( multiboot_magic, multiboot_info );
	initalize_terminal();
	initalize_interrupts();
	initalize_serial();

	debug_f( "VersionTwo Serial Output\n\n" );
	debug_f2( "VersionTwo Log Output\n\n" );

	initalize_memory();
	initalize_user_mode();
	initalize_keyboard();
 
	printf( "           _   _                   _              _____  _    _  _____ \n" );
	printf( "          | | | |                 (_)            |_   _|| |  | ||  _  |\n" );
	printf( "          | | | |  ___  _ __  ___  _   ___   _ __  | |  | |  | || | | |\n" );
	printf( "          | | | | / _ \\| '__|/ __|| | / _ \\ | '_ \\ | |  | |/\\| || | | |\n" );
	printf( "          \\ \\_/ /|  __/| |   \\__ \\| || (_) || | | || |  \\  /\\  /\\ \\_/ /\n" );
	printf( "           \\___/  \\___||_|   |___/|_| \\___/ |_| |_|\\_/   \\/  \\/  \\___/ \n" );
	printf( "\n\n" );

	mem_tests();
	//test_user_mode_app();
	//keyboard_entry_test();
	
	added_function();
	initalize_symbol_table( multiboot_info );
	initalize_command_line();
	run_command_line();

	//peek( addr );


	
	printf( "\nEntering forever land." );

	while ( 1 ) { ; }
}

void added_function( void ) {
	//printf( "I'm added!" );
}

void peek( uint32_t * address ) {
	uint8_t *ptrn = (uint8_t *)address;
	char *ptr = (char *)address;
	bool send_to_debug = true;
	uint32_t lines = 300;

	for( int x = 0; x < lines; x++ ) {
		if( send_to_debug ) {
			debug_f( "0x%08X    %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X    %c%c%c%c%c%c%c%c%c%c\n",
			address + (x * 10), 
			*(ptrn), *(ptrn + 1), *(ptrn + 2), *(ptrn + 3), *(ptrn + 4), *(ptrn + 5), *(ptrn + 6), *(ptrn + 7), *(ptrn + 8), *(ptrn + 9), 
			peek_char( *(ptr) ), peek_char( *(ptr + 1) ), peek_char( *(ptr + 2) ), peek_char( *(ptr + 3) ), peek_char( *(ptr + 4) ), peek_char( *(ptr + 5) ), peek_char( *(ptr + 6) ), peek_char( *(ptr + 7) ), peek_char( *(ptr + 8) ), peek_char( *(ptr + 9) ) );
		} else {
			printf( "0x%08X    %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X    %c%c%c%c%c%c%c%c%c%c\n",
			address + (x * 10), 
			*(ptrn), *(ptrn + 1), *(ptrn + 2), *(ptrn + 3), *(ptrn + 4), *(ptrn + 5), *(ptrn + 6), *(ptrn + 7), *(ptrn + 8), *(ptrn + 9), 
			peek_char( *(ptr) ), peek_char( *(ptr + 1) ), peek_char( *(ptr + 2) ), peek_char( *(ptr + 3) ), peek_char( *(ptr + 4) ), peek_char( *(ptr + 5) ), peek_char( *(ptr + 6) ), peek_char( *(ptr + 7) ), peek_char( *(ptr + 8) ), peek_char( *(ptr + 9) ) );
		}

		ptr = ptr + 10;
		ptrn = ptrn + 10;
	}
}

char peek_char( char c ) {
	if( (c > 31) && (c < 127) ) {
		return c;
	} else {
		return '.';
	}
}