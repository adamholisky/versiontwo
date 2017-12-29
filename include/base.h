typedef void (*putcf) (void*,char);

void putc( void *p, char c );
void update_cursor( int row, int col );
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
bool get_global_can_update_cursor( void );