#include "kernel.h"
#include <stdio.h>

void vtos_cmd_setup_peek( void );
void vtos_cmd_peek( int argc, char ** argv );

void vtos_cmd_setup_peek( void ) {
    add_command( "peek", vtos_cmd_peek );
}

void vtos_cmd_peek( int argc, char ** argv ) {
    char str_address[255];
	char lines = 8;
	bool send_to_debug = false;

	if( argv[1] == NULL ) {
		printf( "Usage: peek <hex address> <lines> <debug>\n" );
		return;
	}

	uint32_t address = hex2int( argv[1], strlen( argv[1] ) );

	if( argc >= 3 ) {
		lines = atoi( argv[2] );
	}

	if( argc >= 4 ) {
		send_to_debug = true;
	}

	printf( "Looking at 0x%08X\n", address );
	
	uint8_t *ptrn = (uint8_t *)address;
	char *ptr = (char *)address;

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

	printf( "\n" );
}