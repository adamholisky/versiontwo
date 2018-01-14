#include "kernel.h"
#include <stdio.h>

extern void system_interrupt_test( void );

void kernel_main( void ) {
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
	test_user_mode_app();
	//keyboard_entry_test();
	
	printf( "\nEntering forever land." );

	while ( 1 ) { ; }
}