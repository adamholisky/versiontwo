/*
 * Interrupt types
 */
#define INT_0 0x8E00     // 1000111000000000 = present,ring0,int_gate
#define INT_3 0xEE00     // 1110111000000000 = present,ring3,int_gate

/*
 * IRQ macros
 */
#define ALL       0xFF
#define TIMER     0
#define KEYBOARD  1
#define CASCADE   2
#define COM2_4    3
#define COM1_3    4
#define LPT       5
#define FLOPPY    6
#define FREE7     7
#define CLOCK     8
#define FREE9     9
#define FREE10    10
#define FREE11    11
#define PS2MOUSE  12
#define COPROC    13
#define IDE_1     14
#define IDE_2     15

/*
 * PIC macros
 * Master = PIC chip 1
 * Slave = PIC chip 2
 */
#define MASTER          0x20
#define MASTERDATA      0x21
#define SLAVE           0xA0
#define SLAVEDATA       0xA1
#define EOI             0x20

/*
 * ICW1 and ICW4 control words are the only ones we have to worry about
 */
#define ICW1_INIT    0x10               // required for PIC initialisation
#define ICW1_EDGE    0x08               // edge triggered IRQs
#define ICW1_SINGLE  0x02               // only MASTER (not cascaded)
#define	ICW1_ICW4    0x01               // there IS an ICW4 control word

#define ICW4_SFNM    0x10               // Special Fully Nested Mode
#define ICW4_BUFFER  0x08               // Buffered Mode
#define ICW4_MASTER  0x04               // this is the Master PIC
#define ICW4_AEOI    0x02               // Auto EOI
#define ICW4_8086    0x01               // 80/86 Mode

/* structure for an interrupt */
typedef struct {
	word            low_offset;                         // low nibble of offset to handler of interrupt 
	word            selector;                           // GDT selector used 
	word            settings;                           // settings for int 
	word            high_offset;                        // high nibble to handler code 
}  __attribute__ ((packed)) x86_interrupt;


/* structure for the IDTR */
typedef struct {
     word           limit;                             // limit or Size of IDT 
     x86_interrupt  *base;                    // a pointer to the base of the IDT 
} __attribute__ ((packed)) idtr;

struct interrupt_stack {
	unsigned int	gs;
	unsigned int	fs;
	unsigned int	es;
	unsigned int	ds;
	unsigned int	edi;
	unsigned int	esi;
	unsigned int	ebp;
	unsigned int	esp;
	unsigned int	ebx;
	unsigned int	edx;
	unsigned int	ecx;
	unsigned int	eax;
	unsigned int 	ss;
	unsigned int	useresp;
	unsigned int    cs;
	unsigned int   	eip;
};

extern void int00(void);
extern void int01(void);
extern void int02(void);
extern void int03(void);
extern void int04(void);
extern void int05(void);
extern void int06(void);
extern void int07(void);
extern void int08(void);
extern void int09(void);
extern void int10(void);
extern void int11(void);
extern void int12(void);
extern void int13(void);
extern void int14(void);
extern void int15(void);
extern void int16(void);
extern void int17(void);
extern void int18(void);
extern void int19(void);
extern void int30(void);
extern void int20( void );
extern void int21( void );
extern void int22( void );
extern void int23( void );
extern void int24( void );
extern void int25( void );
extern void int26( void );
extern void int27( void );
extern void int28( void );
extern void int29( void );
extern void int2A( void );
extern void int2B( void );
extern void int2C( void );
extern void int2D( void );
extern void int2E( void );
extern void int2F( void );

void int_00(void);
void int_01(void);
void int_02(void);
void int_03(void);
void int_04(void);
void int_05(void);
void int_06(void);
void int_07(void);
void int_08( struct interrupt_stack stack );
void int_09(void);
void int_10(void);
void int_11(void);
void int_12(void);
void int_13( struct interrupt_stack stack );
void int_14( void * address );
void int_16(void);
void int_17(void);
void int_18(void);
void int_19(void);
void int_22(void);
void int_23(void);
void int_24(void);
void int_25(void);
void int_26(void);
void int_27(void);
void int_28(void);
void int_29(void);
void int_2A(void);
void int_2B(void);
void int_2D(void);
void int_2E(void);
void int_2F(void);

void initalize_interrupts( void );
void load_idtr( void );//sets up IDT by loading IDTR
void AddInt( int number, void (*handler)(), dword dpl ); //add interrupt
void load_exceptions(); // init all exceptions
void panic(char *message, char *mnemonic, bool halt); //exception panic
void remap_pic(int pic1, int pic2);      //remap PIC
void mask_irq(byte irq);                 //PIC irq masking
void unmaskIRQ(byte irq);               //PIC irq unmasking
void INTS(bool on);                     //sti or cli
void pic_acknowledge(unsigned int interrupt);
void interrupt_default_handler( unsigned long route_code, struct interrupt_stack stack );
void set_debug_interrupts( bool d );

