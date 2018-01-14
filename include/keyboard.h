#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER

void initalize_keyboard( void );
char get_scancode( void );
char get_character( unsigned int scancode );
void keyboard_interrupt_handler( void );
void keyboard_entry_test( void );

#endif