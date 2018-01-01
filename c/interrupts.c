#include "kernel.h"

idtr            sIDTR;
x86_interrupt   IDT[256];			// = (x86_interrupt*)0xD00;
uint8_t	        pic1_irq_mask;
uint8_t	        pic2_irq_mask;
bool	        debug_interrupts = false;
bool 	        gpf_fired = false;

void initalize_interrupts( void ) {
	remap_pic( 0x20, 0x28 );

	int divisor = 1193180 / 100;      // Calculate our divisor
    outportb(0x43, 0x36);             // Set our command byte 0x36
    outportb(0x40, divisor & 0xFF);   // Set low byte of divisor
    outportb(0x40, divisor >> 8);     // Set high byte of divisor

	load_exceptions();

	add_interrupt( 0x20, interrupt_0x20, 0 );
	add_interrupt( 0x21, interrupt_0x21, 0 );
	add_interrupt( 0x22, interrupt_0x22, 0 );
	add_interrupt( 0x23, interrupt_0x23, 0 );
	add_interrupt( 0x24, interrupt_0x24, 0 );
	add_interrupt( 0x25, interrupt_0x25, 0 );
	add_interrupt( 0x26, interrupt_0x26, 0 );
	add_interrupt( 0x27, interrupt_0x27, 0 );
	add_interrupt( 0x28, interrupt_0x28, 0 );
	add_interrupt( 0x29, interrupt_0x29, 0 );
	add_interrupt( 0x2A, interrupt_0x2A, 0 );
	add_interrupt( 0x2B, interrupt_0x2B, 0 );
	add_interrupt( 0x2C, interrupt_0x2C, 0 );
	add_interrupt( 0x2D, interrupt_0x2D, 0 );
	add_interrupt( 0x2E, interrupt_0x2E, 0 );
	add_interrupt( 0x2F, interrupt_0x2F, 0 );
	add_interrupt( 0x30, interrupt_0x30, 3 );

	load_idtr();

	asm( "sti" );

	asm( "int %0" : : "i"(0x2E) );
	asm( "int %0" : : "i"(0x2F) );
	asm( "int %0" : : "i"(0x30) );

}

void load_idtr( void )
{
    sIDTR.limit  = 256*(sizeof(x86_interrupt)-1);
    sIDTR.base   = IDT;

	idtr *IDTRptr = &sIDTR;

    asm volatile("lidt (%0) ": : "r"(IDTRptr));
}

void set_debug_interrupts( bool d ) {
	debug_interrupts = d;
}

void interrupt_default_handler( unsigned long interrupt_num, unsigned long route_code, struct interrupt_stack * stack ) {
	// debug_f( "Interrupt: num: 0x%04X  Route: 0x%04X  eax: 0x%04X\n", interrupt_num, route_code, stack->eax );

	/*  Route codes:
			0x01 == Exceptions
			0x02 == Interrupts (everything else)
	*/

	if ( route_code == 0x01 ) {
		switch( interrupt_num ) {
			case 0:
				debug_f( "Exception: Divide by zero\n" );
				break;
			case 6:
				debug_f( "Exception: Invalid opcode\n" );
				break;
			default:
				debug_f( "Unhandled exception: %0x02X\n", interrupt_num );
		}
		
		while( 1 ) { ; }
	} else {
		if( interrupt_num == 0x30 ) {
			unsigned int eax = (unsigned int)stack->eax;
		
			if( eax == 0x8844 ) {
				debug_f( "System call executed" );
			}

			if( eax == 0x8845 ) {
				debug_f( "!" );

				union x86flags ef;
				
				ef = (union x86flags)stack->eflags;

				printf( "User mode register dump:\n" );
				printf( "  eax:  0x%08X  ebx:  0x%08X  ecx:  0x%08X  edx:  0x%08X\n", stack->eax, stack->ebx, stack->ecx, stack->edx );
				printf( "  esp:  0x%08X  ebp:  0x%08X  esi:  0x%08X  edi:  0x%08X\n", stack->esp, stack->ebp, stack->esi, stack->edi );
				printf( "  ds:   0x%04X      es:   0x%04X      fs:   0x%04X      gs:   0x%04X\n", stack->ds, stack->es, stack->fs, stack->gs );
				printf( "  cs:   0x%04X      eip:  0x%08X    flag: 0x%08X\n", stack->cs, stack->eip, ef.all_flags );
				printf( "  cf: %d  pf: %d  af: %d  zf: %d  sf: %d  tf: %d  if: %d  iopl: %d  nt: %d  rf: %d\n", ef.eflags_bits.cf, ef.eflags_bits.pf, ef.eflags_bits.af, ef.eflags_bits.zf, ef.eflags_bits.sf, ef.eflags_bits.tf, ef.eflags_bits.ifen, ef.eflags_bits.iopl, ef.eflags_bits.nt, ef.eflags_bits.rf );
				printf( "  vm: %d  ac: %d  vif: %d  id: %d\n\n", ef.eflags_bits.vm, ef.eflags_bits.ac, ef.eflags_bits.vif, ef.eflags_bits.id );

				// Set this to prove we can modify program dynamically 
				stack->ebx = stack->ebx + 1;
			}

			pic_acknowledge( interrupt_num );
		}
	}
}

void add_interrupt( int number, void (*handler)(), dword dpl ) {
	 word selector = 0x08;
	 word settings;
     dword offset = (dword)handler;

	 /* get CS selector */
     asm volatile("movw %%cs,%0" :"=g"(selector));

	 /* set settings options depending on dpl */
	 switch(dpl) {
	 	case 0: 
			settings = INT_0;
			break;
	 	case 1:
	 	case 2:
	 	case 3: 
			settings = INT_3;
			break;
	 }
	 
	 /* set actual values of int */
	 IDT[number].low_offset   = (offset & 0xFFFF);
	 IDT[number].selector     = selector;
	 IDT[number].settings     = settings;
     IDT[number].high_offset  = (offset >> 16);
}

void load_exceptions()
{
	/*
	 * Add all Exception Interrupts
	 */
	add_interrupt(0, interrupt_0, 0);
    add_interrupt(1, interrupt_1, 0);
    add_interrupt(2, interrupt_2, 0);
    add_interrupt(3, interrupt_3, 0);
    add_interrupt(4, interrupt_4, 0);
    add_interrupt(5, interrupt_5, 0);
    add_interrupt(6, interrupt_6, 0);
    add_interrupt(7, interrupt_7, 0);
    add_interrupt(8, interrupt_8, 0);
    add_interrupt(9, interrupt_9, 0);
    add_interrupt(10, interrupt_10, 0);
    add_interrupt(11, interrupt_11, 0);
    add_interrupt(12, interrupt_12, 0);
    add_interrupt(13, interrupt_13, 0);
    add_interrupt(14, interrupt_14, 0);
    add_interrupt(16, interrupt_16, 0);
    add_interrupt(17, interrupt_17, 0);
    add_interrupt(18, interrupt_18, 0);
    add_interrupt(19, interrupt_19, 0);
}

 /* enables/disables software interrupts */
void INTS(bool on) {
	if( on ) {
		asm("sti");
	} else {
		asm("cli");
	}
}

void pic_acknowledge( unsigned int interrupt ) {
	if( interrupt > 0x28 ) {
		outportb( 0xA0, 0x20 );
	}

	outportb( 0x20, 0x20 );
}

void panic( char *message, char *code, bool halt ) {
     debug_f( message );
     
	 outportb(MASTER, EOI); //send PIC EOI command

	 while ( 1 ) { ; }
}

void int_13( struct interrupt_stack stack ) {
	if( ! gpf_fired ) {
		debug_f( "GPF eip: 0x%08X\n", stack.eip );
		gpf_fired = true;
	}
}

void interrupt_handler_14( void * address ) {
	//write_to_serial_port( '*' );
	debug_f( "PF: 0x%08X\n", address );
	//panic("Page Fault","#PF", false);
}

void remap_pic(int pic1, int pic2) {
	byte md,sd;

	md=inportb(MASTERDATA);                     // save state of MASTER DATA
	sd=inportb(SLAVEDATA);                      // save state of SLAVE DATA

	outportb(MASTER, EOI);                      // Send EOI | resets the chip

	outportb(MASTER, ICW1_INIT+ICW1_ICW4);      // ICW1 control word setup | just basic PIC stuff
	outportb(SLAVE, ICW1_INIT+ICW1_ICW4);       // see pic.h for more details about the values

	outportb(MASTERDATA, pic1);                 // ICW2 maps IRQs 0-7 to whatever kernel passes
	outportb(SLAVEDATA, pic2);                  // and same here except with IRQs 8-15 

	outportb(MASTERDATA, 0x04);                 // ICW3 
	outportb(SLAVEDATA, 0x02);

	outportb(MASTERDATA, ICW4_8086);            // ICW4 control word setup
	outportb(SLAVEDATA, ICW4_8086);

	outportb(MASTERDATA,md);                    // restore both MASTER DATA
	outportb(SLAVEDATA,sd);                     // restore SLAVE DATA
}
	
/* makes IRQ unavailable so it can't fire an interrupt */
void mask_irq( byte irq ) {
	if( irq==ALL ) {
		outportb(MASTERDATA,0xFF);
		outportb(SLAVEDATA,0xFF);
		pic1_irq_mask = 0xFF;
		pic2_irq_mask = 0xFF;
	} else {
		irq = irq | (1<<irq);
		if(irq < 8)
			outportb(MASTERDATA, irq&0xFF);
		else
			outportb(SLAVEDATA, irq>>8);
	}
}

/* opposite of above */
void unmaskIRQ( byte irq ) {
	if( irq==ALL ) {
		outportb(MASTERDATA,0x00);
		outportb(SLAVEDATA,0x00);
		pic1_irq_mask = 0x00;
		pic2_irq_mask = 0x00;
	} else {
		//irq = irq & (1<<irq);

		if(irq < 8) {
			pic1_irq_mask = pic1_irq_mask ^ (1<<irq);
			outportb(MASTERDATA, pic1_irq_mask );
		}
		else {
			pic2_irq_mask = pic2_irq_mask ^ (1<<irq);
			outportb(SLAVEDATA, pic2_irq_mask);
		}
	}
}