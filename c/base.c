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

int64_t __divdi3(int64_t num, int64_t den)
 {
   return __divmoddi4(num, den, NULL);
 }

int64_t __divmoddi4(int64_t num, int64_t den, int64_t *rem_p)
 {
   int minus = 0;
   int64_t v;
 
   if ( num < 0 ) {
     num = -num;
     minus = 1;
   }
   if ( den < 0 ) {
     den = -den;
     minus ^= 1;
   }
 
   v = __udivmoddi4(num, den, (uint64_t *)rem_p);
   if ( minus ) {
     v = -v;
     if ( rem_p )
       *rem_p = -(*rem_p);
   }
 
   return v;
 }

int64_t __moddi3(int64_t num, int64_t den)
 {
   int64_t v;
 
   (void) __divmoddi4(num, den, &v);
   return v;
 }

uint64_t __udivdi3(uint64_t num, uint64_t den)
 {
   return __udivmoddi4(num, den, NULL);
 }

uint64_t __udivmoddi4(uint64_t num, uint64_t den, uint64_t *rem_p)
 {
   uint64_t quot = 0, qbit = 1;
 
   if ( den == 0 ) {
     return 1/((unsigned)den); /* Intentional divide by zero, without
                                  triggering a compiler warning which
                                  would abort the build */
   }
 
   /* Left-justify denominator and count shift */
   while ( (int64_t)den >= 0 ) {
     den <<= 1;
     qbit <<= 1;
   }
 
   while ( qbit ) {
     if ( den <= num ) {
       num -= den;
       quot += qbit;
     }
     den >>= 1;
     qbit >>= 1;
   }
 
   if ( rem_p )
     *rem_p = num;
 
   return quot;
 }

uint64_t __umoddi3(uint64_t num, uint64_t den)
 {
   uint64_t v;
 
   (void) __udivmoddi4(num, den, &v);
   return v;
 }

 void abort(void)
{
    //raise(SIGABRT);
}

void __builtin_huge_inf( ) {
	return 0;
}

void free( void * p ) {
	return;
}