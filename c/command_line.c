#include "kernel.h"
#include <stdio.h>
#include <string.h>

#define __builtin_strtok( a, b ) strtok( a, b )

typedef struct {
	void * entry_addr;
	char name[256];
} command_entry;

char * command_line;
char command_args[20][256];
uint32_t num_cmds;
uint32_t current_cmd;
command_entry * cmds;

void initalize_command_line( void ) {
    current_cmd = 0;
    command_line = malloc( sizeof( char ) * 256 );
    char command_prefix[] = "vtos_cmd_setup";
    kernel_symbols * ksym = get_ksym();
    uint32_t num_syms = get_num_syms();
    void (*cmd_setup)( void ) = NULL;

    char command_temp[ 255 ];

    for( int z; z < num_syms; z++ ) {
        strcpy( command_temp, ksym[ z ].name );
        command_temp[14] = 0;
        int result = strcmp( command_temp, "vtos_cmd_setup" );

        //debug_f( "%s -- 0x%08X\n", command_temp, result );

        if( result == 0 ) {
            num_cmds++;
            debug_f( "%s() is located at 0x%08X\n", ksym[ z ].name, ksym[ z ].entry_addr );
        }
    }

    cmds = malloc( sizeof( command_entry ) * num_cmds );

    for( int z; z < num_syms; z++ ) {
        strcpy( command_temp, ksym[ z ].name );
        command_temp[14] = 0;
        int result = strcmp( command_temp, "vtos_cmd_setup" );


        if( result == 0 ) {
            cmd_setup = ksym[ z ].entry_addr;
            cmd_setup();
        }
    }
}

void add_command( char * cmd_name, void * addr ) {
    strcpy( cmds[ current_cmd ].name, cmd_name );
    cmds[ current_cmd ].entry_addr = addr;
    current_cmd++;
}

void run_command_line( void ) {
    int i, j, n, x;
    char * token_ptr;
    char * tokens[20];
    void (*cmd_entry_point)( int, char ** ) = NULL;

    for( n = 0; n < 21; n++ ) {
        *(tokens + n) = malloc( sizeof( char ) * 255 );
    }

    while ( 1 ) {
        j = 0;

        for( x = 0; x < 20; x++ ) {
            *(tokens[ x ]) = 0;
        }

        printf( "vtos: " );
    	get_string( command_line, 256 );

        token_ptr = strtok( command_line, " ," );
 
		while( token_ptr != NULL && j < 20 ) {
			strcpy( tokens[ j ], token_ptr );
            tokens[ j ][ strlen( token_ptr ) + 1 ] = 0;
			j++;
			token_ptr = strtok( NULL, " ," );
            //debug_f( "token: %s\n", tokens[ j - 1 ] );
		}

        for( i = 0; i < num_cmds; i++ ) {
            if( strcmp( tokens[ 0 ], cmds[ i ].name ) == 0 ) {
                cmd_entry_point = cmds[ i ].entry_addr;
                cmd_entry_point( j, tokens );
            }
        }
    }
}