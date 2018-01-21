#if !defined(CMD_LINE_INCLUDED)
#define CMD_LINE_INCLUDED

void initalize_command_line( void );
void run_command_line( void );
void add_command( char * cmd_name, void * addr );

#endif