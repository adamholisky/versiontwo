#include "kernel.h"

bool global_can_update_cursor = false;

/* Basic putc wrapper for printf() to put a char on the screen
*/
void putc( void *p, char c ) {
	void *file_to_write_to = p;

// Kill the gcc warning of unused var since we're not supporting different streams right now
	file_to_write_to++;

	term_put_char( c );
}


void update_cursor( int row, int col ) {
	if( global_can_update_cursor ) {
		if( !is_gui_active() ) {
			unsigned short position=(row*80) + col;

			// cursor LOW port to vga INDEX register
			outportb(0x3D4, 0x0F);
			outportb(0x3D5, (unsigned char)(position&0xFF));

			// cursor HIGH port to vga INDEX register
			outportb(0x3D4, 0x0E);
			outportb(0x3D5, (unsigned char )((position>>8)&0xFF));
		} else {
			//gui_update_carrot();
		}
	}
}

