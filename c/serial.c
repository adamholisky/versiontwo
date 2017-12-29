#include "kernel.h"

uint32_t	port;

uint32_t COM1 = 0x3F8;
uint32_t COM2 = 0x2F8;
uint32_t COM3 = 0x3E8;
uint32_t COM4 = 0x2E8;

void serial_setup( void ) {
	port = COM1;

	outportb( port + 1, 0x00);    // Disable all interrupts
	outportb( port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outportb( port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outportb( port + 1, 0x00);    //                  (hi byte)
	outportb( port + 3, 0x03);    // 8 bits, no parity, one stop bit
	outportb( port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outportb( port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

void serial_write( char c ) {
	// Make sure the transmit queue is empty
	while( ( inportb( port + 5) & 0x20 ) == 0 ) {
		;
	}
 
	outportb( port, c );
}

char serial_read( void ) {
	// Wait until we can get something
	while( ( inportb( port + 5) & 1 ) == 0 ) {
		;
	}
 
	return inportb( port );
}
