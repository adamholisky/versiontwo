/*

   Copyright 2009 Pierre KRIEGER

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   
*/

#ifndef _STDC_STDIO_H_
#define _STDC_STDIO_H_

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*putcf) (void*,char);

int		snprintf		(char* s, size_t n, const char* format, ...);
int		sprintf		(char* s, const char* format, ...);
int		sscanf		(const char* s, const char* format, ...);
int		vsnprintf		(char* s, size_t n, const char* format, va_list arg);
int		vsprintf		(char* s, const char* format, va_list arg);
int		vsscanf		(const char* s, const char* format, va_list arg);
int getchar( void );
int scanf( const char* format, ... );
void get_string( char * string, unsigned int size );

void init_printf(void* putp,void (*putf) (void*,char));
void tfp_printf(char *fmt, ...);
void tfp_sprintf(char* s,char *fmt, ...);
void tfp_format(void* putp,void (*putf) (void*,char),char *fmt, va_list va);
#define printf tfp_printf 
#define sprintf tfp_sprintf 

void printfcomma2 (int n);
void printfcomma (int n);
static char a2i(char ch, char** src,int base,int* nump);

void * get_stdout_putp( void );
putcf get_stdout_putf( void );

void con_printf_secondary( char *fmt, va_list va );
void debug_f( char *fmt, ...);
unsigned long hex2int(char *a, unsigned int len);
char * to_lower( char * str );
	
#ifdef __cplusplus
}
#endif


#endif
