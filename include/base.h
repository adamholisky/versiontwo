#define debug_level_one() serial_set_default_port( COM1 )
#define debug_level_two() serial_set_default_port( COM2 )
#define debug_f2( ... ) debug_level_two(); debug_f( __VA_ARGS__ ); debug_level_one()



void putc( void *p, char c );
void update_cursor( int row, int col );


bool get_global_can_update_cursor( void );