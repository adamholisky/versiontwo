#if !defined(BASE_INCLUDED)
#define BASE_INCLUDED

#define debug_level_one() serial_set_default_port( COM1 )
#define debug_level_two() serial_set_default_port( COM2 )
#define debug_f2( ... ) debug_level_two(); debug_f( __VA_ARGS__ ); debug_level_one()

#define vtd1 printf( "D1" )
#define vtd2 printf( "D2" )
#define vtd3 printf( "D3" )
#define vtd4 printf( "D4" )
#define vtd5 printf( "D5" )



void putc( void *p, char c );
void update_cursor( int row, int col );


bool get_global_can_update_cursor( void );

#ifndef _LIBGCC_H
#define _LIBGCC_H

#include <stdint.h>
#include <stddef.h>

uint64_t __udivmoddi4 ( uint64_t num, uint64_t den,
                                        uint64_t *rem );
uint64_t __udivdi3  (uint64_t num, uint64_t den );
uint64_t __umoddi3 ( uint64_t num, uint64_t den );
int64_t __divmoddi4 ( int64_t num, int64_t den, int64_t *rem );
int64_t __divdi3 ( int64_t num, int64_t den );
int64_t __moddi3 ( int64_t num, int64_t den );

#endif /* _LIBGCC_H */


void abort(void);
void __builtin_huge_inf( );
void free( void * p );

#endif