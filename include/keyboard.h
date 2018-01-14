#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER

void initalize_keyboard( void );
char get_scancode( void );
char get_character( unsigned int scancode );

#endif