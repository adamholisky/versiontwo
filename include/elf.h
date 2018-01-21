#if !defined(ELF_INCLUDED)
#define ELF_INCLUDED

typedef uint16_t Elf32_Half;	// Unsigned half int
typedef uint32_t Elf32_Off;	// Unsigned offset
typedef uint32_t Elf32_Addr;	// Unsigned address
typedef uint32_t Elf32_Word;	// Unsigned int
typedef int32_t  Elf32_Sword;	// Signed int

typedef struct {
	void *		entry_addr;
	char *		name;
} kernel_symbols;

typedef struct {
	Elf32_Word	sh_name;
	Elf32_Word	sh_type;
	Elf32_Word	sh_flags;
	Elf32_Addr	sh_addr;
	Elf32_Off	sh_offset;
	Elf32_Word	sh_size;
	Elf32_Word	sh_link;
	Elf32_Word	sh_info;
	Elf32_Word	sh_addralign;
	Elf32_Word	sh_entsize;
} Elf32_Shdr;

typedef struct {
	Elf32_Word		st_name;
	Elf32_Addr		st_value;
	Elf32_Word		st_size;
	uint8_t			st_info;
	uint8_t			st_other;
	Elf32_Half		st_shndx;
} Elf32_Sym;

 
enum ShT_Types {
	SHT_NULL	= 0,   // Null section
	SHT_PROGBITS	= 1,   // Program information
	SHT_SYMTAB	= 2,   // Symbol table
	SHT_STRTAB	= 3,   // String table
	SHT_RELA	= 4,   // Relocation (w/ addend)
	SHT_NOBITS	= 8,   // Not present in file
	SHT_REL		= 9,   // Relocation (no addend)
};

kernel_symbols * get_ksym( void );
uint32_t get_num_syms( void );

#endif