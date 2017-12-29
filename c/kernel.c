#include "kernel.h"

void kernel_main(void) {
	term_setup();
	initalize_interrupts();
	serial_setup();
 
	printf( "VersionTwo\n" );

	debug_f( "Hello, serial port...\n\nNice to see you!\n" );

	term_write_string( "Ending" );
}