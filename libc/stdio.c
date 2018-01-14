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

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "keyboard.h"
#include "term.h"

char stdin_buffer[2048];

int snprintf(char* str, size_t size, const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int n = vsnprintf(str, size, format, ap);
	va_end(ap);
	return n;
}

/*
int sprintf(char* str, const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int n = vsprintf(str, format, ap);
	va_end(ap);
	return n;
}
*/

int sscanf(const char* s, const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int n = vsscanf(s, format, ap);
	va_end(ap);
	return n;
}

int vsnprintf(char* str, size_t size, const char* format, va_list ap) {
	// TODO: 'size' is not handled
	// TODO: 'e', 'f', 'g', 'n' specifiers to do
	// TODO: function doesn't return the number of printed characters
	
	if (size == 0)
		return 0;
	
	// we loop through each character of the format
	while (*format != '\0' && size > 1) {
		// first we handle the most common case: a normal character
		if (*format != '%') {
			*str++ = *format++;
			continue;
		}
		
		// then we check if format is "%%"
		format++;
		if (*format == '%') {
			*str++ = '%';
			format++;
			continue;
		}
		
		// now we are sure we are in a special case
		// what we do is that we store flags, width, precision, length in variables
		bool sharpFlag = false;
		bool alignLeft = false;
		bool alwaysSign = false;
		bool noSign = false;
		bool padding = ' ';
		int minimumWidth = 0;
		int precision = 1;
		bool numberMustBeShort = false;
		bool numberMustBeLong = false;
		bool unsignedNumber = false;
		bool capitalLetters = false;
		bool octal = false;
		bool hexadecimal = false;
		bool pointer = false;
		bool tagFinished = false;
		
		// then we loop (and we modify variables) until we find a specifier
		do {
			
			switch (*format) {
				// flags
				case '-': alignLeft = true; 		format++; break;
				case '+': alwaysSign = true;		format++; break;
				case ' ': noSign = true;		format++; break;
				case '0': padding = '0';			format++; break;
				case '#': sharpFlag = true;		format++; break;
				
				// width
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':		// width cannot start with 0 or it would be a flag
				case '6':
				case '7':
				case '8':
				case '9':
					minimumWidth = atoi(format);
					while (*format >= '0' && *format <= '9') format++;
					break;
				case '*':
					minimumWidth = va_arg(ap, int);
					format++;
					break;
					
				// precision
				case '.':
					format++;
					if (*format == '*') {
						precision = va_arg(ap, int);
						format++;
					} else if (*format >= '0' && *format <= '9') {
						precision = atoi(format);
						while (*format >= '0' && *format <= '9') format++;
					} else {
						precision = 0;		// this behavior is standardized
					}
					break;
					
				// length
				case 'h': numberMustBeShort = true;	format++; break;
				case 'l':
				case 'L': numberMustBeLong = true;	format++; break;
				
				// specifiers
				
				
				//	strings
				case 's': {
					char* nStr = va_arg(ap, char*);
					size_t len = strlen(nStr);
					
					if (!alignLeft && len < minimumWidth) {
						while (len++ < minimumWidth)
							*str++ = padding;
					}
					
					while (*nStr)
						*str++ = *nStr++;
					
					if (alignLeft && len < minimumWidth) {
						while (len++ < minimumWidth)
							*str++ = padding;
					}
					
					format++;
					tagFinished = true;
					break;
				}
				
				
				
				// 	characters
				case 'c': {
					char toWrite;
					/*if (numberMustBeLong)		toWrite = (char)va_arg(ap, wchar_t);
					else					*/toWrite = (char)va_arg(ap, int);
					
					if (!alignLeft) {
						for (; minimumWidth > 1; minimumWidth--)
							*str++ = padding;
					}
					
					*str++ = toWrite;
					
					if (alignLeft) {
						for (; minimumWidth > 1; minimumWidth--)
							*str++ = padding;
					}
					
					format++;
					tagFinished = true;
					break;
				}
				
				
				// 	numbers
				case 'o':	octal = true;
				case 'p':	pointer = true;
				case 'X':	capitalLetters = true;
				case 'x':	hexadecimal = true;
				case 'u':	unsignedNumber = true;
				case 'd':
				case 'i': {
					// first we handle problems with our switch-case
					if (octal) { pointer = false; hexadecimal = false; unsignedNumber = false; }
					
					// then we retreive the value to write
					unsigned long int toWrite;
					if (numberMustBeLong)			toWrite = va_arg(ap, long int);
					else if (numberMustBeShort)		toWrite = (short int)va_arg(ap, int);
					else if (pointer)				toWrite = (unsigned long int)va_arg(ap, void*);
					else						toWrite = va_arg(ap, int);
					
					// handling sign
					if (!noSign) {
						bool positive = (unsignedNumber || (((signed)toWrite) > 0));
						if (alwaysSign || !positive)
							*str++ = (positive ? '+' : '-');
						if (!unsignedNumber && (((signed)toWrite) < 0))
							toWrite = -((signed)toWrite);
					}
					
					if (sharpFlag && toWrite != 0) {
						if (octal || hexadecimal)
							*str++ = '0';
						if (hexadecimal) {
							if (capitalLetters)	*str++ = 'X';
							else				*str++ = 'x';
						}
					}
					
					// writing number
					int digitSwitch = 10;
					if (hexadecimal)	digitSwitch = 16;
					else if (octal)	digitSwitch = 8;
					
					// this variable will be usefull
					char* baseStr = str;
					
					int numDigits = 0;
					do {
						if (numDigits)
							memmove(baseStr + 1, baseStr, numDigits * sizeof(char));
						int modResult = toWrite % digitSwitch;
						if (modResult < 10)	{	*baseStr = '0' + modResult;			str++;	}
						else if (capitalLetters)	{	*baseStr = 'A' + (modResult - 10);		str++;	}
						else				{	*baseStr = 'a' + (modResult - 10);		str++;	}
						toWrite /= digitSwitch;
						numDigits++;
					} while (toWrite != 0);
					
					if (numDigits < minimumWidth) {
						minimumWidth -= numDigits;
						if (alignLeft) {
							for (; minimumWidth > 0; minimumWidth--)
								*str++ = padding;
						} else {
							memmove(baseStr + minimumWidth * sizeof(char), baseStr, numDigits * sizeof(char));
							memset(baseStr, padding, minimumWidth * sizeof(char));
							str += minimumWidth;
						}
					}
					
					// finished
					format++;
					tagFinished = true;
					break;
				}
				
				default:
					format++;
					tagFinished = true;
					break;
				
			}
		} while (!tagFinished);
	}
	
	*str = '\0';
	
	return 1;
}

int vsprintf(char* str, const char* format, va_list ap) {
	return vsnprintf(str, (size_t)-1, format, ap);
}

int getchar( void ) {
	char scancode;
	char user_char = 0;
	bool done = false;

	scancode = get_scancode();

	while( scancode != 0x1C && done != true ) {
		if( scancode != 0 ) {
			user_char = get_character( (unsigned int)scancode );

			switch( user_char ) {
				case 0:
					break;
				default:
					term_put_char( user_char );
					done = true;
			}
		}

		if( done != true ) {
			scancode = get_scancode();
		}
	}

	return user_char;
}

int scanf( const char* format, ... ) {
	get_string( stdin_buffer, 2048 );

	va_list ap;
	va_start(ap, format);
	int n = vsscanf( stdin_buffer, format, ap);
	va_end(ap);
	return n;
}

void get_string( char * string, unsigned int size ) {
	char scancode;
	char user_char;
	unsigned int x;

	scancode = get_scancode();
	x = 0;

	//global_can_update_cursor = true;

	//printf( "string *: %X\n", (void *)string );

	while( scancode != 0x1C && x != size - 1) {
		if( scancode != 0 ) {
			user_char = get_character( (unsigned int)scancode );

			switch( user_char ) {
				case 0:
					break;
				case '\b':
					if( x > 0 ) {
						term_clear_last_char();
						string[ x ] = '\0';
						x--;
					}
					break;
				default:
					term_put_char( user_char );
					string[ x ] = user_char;

					x++;
			}
		}

		scancode = get_scancode();
	}

	//global_can_update_cursor = false;

	printf( "\n" );

	string[ x ] = 0;
}

/* Begin tiny printf() implementation
* Original Source and Credit: http://www.sparetimelabs.com/tinyprintf/tinyprintf.php
*/
static putcf stdout_putf;
static void* stdout_putp;
static void ui2a(unsigned int num, unsigned int base, int uc,char * bf)
{
	int n=0;
	unsigned int d=1;
	while (num/d >= base)
		d*=base;        
	while (d!=0) {
		int dgt = num / d;
		num%= d;
		d/=base;
		if (n || dgt>0 || d==0) {
			*bf++ = dgt+(dgt<10 ? '0' : (uc ? 'A' : 'a')-10);
			++n;
		}
	}
	*bf=0;
}

void * get_stdout_putp( void ) {
	return stdout_putp;
}

putcf get_stdout_putf( void ) {
	return stdout_putf;
}

static void i2a (int num, char * bf)
{
	if (num<0) {
		num=-num;
		*bf++ = '-';
	}
	ui2a(num,10,0,bf);
}

static int a2d(char ch)
{
	if (ch>='0' && ch<='9') 
		return ch-'0';
	else if (ch>='a' && ch<='f')
		return ch-'a'+10;
	else if (ch>='A' && ch<='F')
		return ch-'A'+10;
	else return -1;
}


static void putchw(void* putp,putcf putf,int n, char z, char* bf)
{
	char fc=z? '0' : ' ';
	char ch;
	char* p=bf;
	while (*p++ && n > 0)
		n--;
	while (n-- > 0) 
		putf(putp,fc);
	while ((ch= *bf++))
		putf(putp,ch);
}

void tfp_format(void* putp,putcf putf,char *fmt, va_list va)
{
	char bf[12];

	char ch;


	while ((ch=*(fmt++))) {
		if (ch!='%') 
			putf(putp,ch);
		else {
			char lz=0;
#ifdef  PRINTF_LONG_SUPPORT
			char lng=0;
#endif
			int w=0;
			ch=*(fmt++);
			if (ch=='0') {
				ch=*(fmt++);
				lz=1;
			}
			if (ch>='0' && ch<='9') {
				ch=a2i(ch,&fmt,10,&w);
			}
#ifdef  PRINTF_LONG_SUPPORT
			if (ch=='l') {
				ch=*(fmt++);
				lng=1;
			}
#endif
			switch (ch) {
				case 0: 
				goto abort;
				case 'u' : {
#ifdef  PRINTF_LONG_SUPPORT
					if (lng)
						uli2a(va_arg(va, unsigned long int),10,0,bf);
					else
#endif
						ui2a(va_arg(va, unsigned int),10,0,bf);
					putchw(putp,putf,w,lz,bf);
					break;
				}
				case 'd' :  {
#ifdef  PRINTF_LONG_SUPPORT
					if (lng)
						li2a(va_arg(va, unsigned long int),bf);
					else
#endif
						i2a(va_arg(va, int),bf);
					putchw(putp,putf,w,lz,bf);
					break;
				}
				case 'x': case 'X' : 
#ifdef  PRINTF_LONG_SUPPORT
				if (lng)
					uli2a(va_arg(va, unsigned long int),16,(ch=='X'),bf);
				else
#endif
					ui2a(va_arg(va, unsigned int),16,(ch=='X'),bf);
				putchw(putp,putf,w,lz,bf);
				break;
				case 'c' : 
				putf(putp,(char)(va_arg(va, int)));
				break;
				case 's' : 
				putchw(putp,putf,w,0,va_arg(va, char*));
				break;
				case '%' :
				putf(putp,ch);
				default:
				break;
			}
		}
	}
	abort:;
}


void init_printf(void* putp,void (*putf) (void*,char))
{
	stdout_putf=putf;
	stdout_putp=putp;
}

void tfp_printf( char *fmt, ...)
{
	va_list va;
	va_start(va,fmt);
	tfp_format(stdout_putp,stdout_putf,fmt,va);
	va_end(va);
}

void con_printf_secondary( char *fmt, va_list va ) {
	tfp_format(stdout_putp,stdout_putf,fmt,va);
}

static void putcp(void* p,char c)
{
	*(*((char**)p))++ = c;
}

void tfp_sprintf(char* s,char *fmt, ...)
{
	va_list va;
	va_start(va,fmt);
	tfp_format(&s,putcp,fmt,va);
	putcp(&s,0);
	va_end(va);
}
/* End tiny printf() implementation */

	void printfcomma2 (int n) {
		if (n < 1000) {
			printf ("%d", n);
			return;
		}
		printfcomma2 (n/1000);
		printf (",%03d", n%1000);
	}

	void printfcomma (int n) {
		if (n < 0) {
			printf ("-");
			n = -n;
		}
		printfcomma2 (n);
	}

static char a2i(char ch, char** src,int base,int* nump)
{
	char* p= *src;
	int num=0;
	int digit;
	while ((digit=a2d(ch))>=0) {
		if (digit>base) break;
		num=num*base+digit;
		ch=*p++;
	}
	*src=p;
	*nump=num;
	return ch;
}

void debug_f( char *fmt, ...) {
	set_debug_output( true );
	va_list va;
	va_start(va,fmt);
	tfp_format(stdout_putp,stdout_putf,fmt,va);
	va_end(va);
	set_debug_output( false );
}

unsigned long hex2int(char *a, unsigned int len)
{
	int i;
	unsigned long val = 0;

	for(i=0;i<len;i++)
	   if(a[i] <= 57)
		val += (a[i]-48)*(1<<(4*(len-1-i)));
	   else
		val += (a[i]-55)*(1<<(4*(len-1-i)));

	return val;
}

uint16_t inportw(uint16_t portid)
{
	uint16_t ret;
	asm volatile("inw %%dx, %%ax":"=a"(ret):"d"(portid));
	return ret;
}

char * to_lower( char * str ) {
	if( str != NULL ) {
		while( *str != '\0' ) {
			if( *str >= 65 && *str <= 90 ) {
				*str = *str + 32;
			}

			str++;
		}
	}

	return str;
}
