#include "kernel.h"

extern void system_interrupt_test( void );

/* Agenda:

	1. done - terminal, printf, debug_f, serial
	2. done - interrupts
	3. done - move kernel to upper half
	4. done - basic system call via interrupt
	5. done - paging
	6. done - user mode
	7. done - load binary into user mode, run it
	8. pre-emptive multi tasking in user mode
*/

void kernel_main( void ) {
	term_setup();
	initalize_interrupts();
	serial_setup();

	initalize_memory();
	initalize_paging();
	initalize_user_mode();
 
	printf( "VersionTwo\n\n" );

	debug_f( "VersionTwo Serial Output\n\n" );

	test_user_mode_app();

	printf( "Entering forever land." );

	while ( 1 ) { ; }
}
