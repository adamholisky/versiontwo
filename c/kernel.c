#include "kernel.h"
#include <stdio.h>

extern void system_interrupt_test( void );

void kernel_main( void ) {
	term_setup();
	initalize_interrupts();
	serial_setup();
	debug_f( "VersionTwo Serial Output\n\n" );

	initalize_memory();
	initalize_user_mode();

	initalize_keyboard();
 
	printf( "VersionTwo\n\n" );

	

	//test_user_mode_app();

	mem_tests();

	char * my_string = kmalloc( sizeof( char ) * 256 );
	int my_number = 0;

	printf( "Enter a word and a number: " );
	scanf( "%s %d", my_string, &my_number );
	printf( "You entered: %s %d\n", my_string, my_number );

	printf( "Enter a character: " );
	char c = getchar();
	printf( "\nYou hit: %c\n", c );

	printf( "\nEntering forever land." );

	while ( 1 ) { ; }
}