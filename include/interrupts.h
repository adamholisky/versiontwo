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
	uint32_t	gs;
	uint32_t	fs;
	uint32_t	es;
	uint32_t	ds;
	uint32_t	edi;
	uint32_t	esi;
	uint32_t	ebp;
	uint32_t	_esp;
	uint32_t	ebx;
	uint32_t	edx;
	uint32_t	ecx;
	uint32_t	eax;

	//uint32_t	error;
	uint32_t	eip;
	uint16_t	cs;
	uint16_t	padding;
	uint32_t	eflags;

	uint32_t	esp;
	uint32_t	ss;
};

union x86flags {
	uint32_t	all_flags;

	struct {
		uint32_t cf:1; 
		uint32_t must_be_1:1;
		uint32_t pf:1;
		uint32_t must_be_0_1:1;
		uint32_t af:1; 
		uint32_t must_be_0_2:1;
		uint32_t zf:1;
		uint32_t sf:1;
		uint32_t tf:1;  
		uint32_t ifen:1;  
		uint32_t df:1;
		uint32_t of:1;
		uint32_t iopl:2; 
		uint32_t nt:1;
		uint32_t must_be_0_3:1;
		uint32_t rf:1; 
		uint32_t vm:1;
		uint32_t ac:1;
		uint32_t vif:1;
		uint32_t vip:1; 
		uint32_t id:1;   
		uint32_t must_be_0_4:2;  
		uint32_t must_be_0_5:4; 
	} eflags_bits;
};

extern void interrupt_0(void);
extern void interrupt_1(void);
extern void interrupt_2(void);
extern void interrupt_3(void);
extern void interrupt_4(void);
extern void interrupt_5(void);
extern void interrupt_6(void);
extern void interrupt_7(void);
extern void interrupt_8(void);
extern void interrupt_9(void);
extern void interrupt_10(void);
extern void interrupt_11(void);
extern void interrupt_12(void);
extern void interrupt_13(void);
extern void interrupt_14(void);
extern void interrupt_16(void);
extern void interrupt_17(void);
extern void interrupt_18(void);
extern void interrupt_19(void);
extern void interrupt_0x30(void);
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


void interrupt_handler_14( void * address );

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
void interrupt_default_handler( unsigned long interrupt_num, unsigned long route_code, struct interrupt_stack * stack );
void set_debug_interrupts( bool d );

