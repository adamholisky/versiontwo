#include "kernel.h"

void vtos_cmd_setup_shutdown( void );
void cmd_shutdown( void );

void vtos_cmd_setup_shutdown( void ) {
    add_command( "q", cmd_shutdown );
}

void cmd_shutdown( void ) {
	debug_f( "Shutting down.\n\n" );
	outportb( 0xF4, 0x00 );
}