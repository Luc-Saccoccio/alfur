#include <stdint.h>

// ELF Header

#define EI_NIDENT 16

typedef struct {
    uint8_t  e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf64_Ehdr;

// e_ident fields and values

#define EI_MAG0 0
#define ELFMAG0 0x7f

#define EI_MAG1 1
#define ELFMAG1 'E'

#define EI_MAG2 2
#define ELFMAG2 'L'

#define EI_MAG3 3
#define ELFMAG3 'F'

#define EI_CLASS 4
#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2

#define EI_DATA 5
#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define EI_VERSION 6

#define EI_OSABI 7
#define ELFOSABI_SYSV       0
#define ELFOSABI_HPUX       1
#define ELFOSABI_NETBSD     2
#define ELFOSABI_LINUX      3
#define ELFOSABI_HURD       4
#define ELFOSABI_SOLARIS    6
#define ELFOSABI_AIX        7
#define ELFOSABI_IRIX       8
#define ELFOSABI_FREEBSD    9
#define ELFOSABI_TRU64      10
#define ELFOSABI_MODESTO    11
#define ELFOSABI_OPENBSD    12
#define ELFOSABI_OPENVMS    13
#define ELFOSABI_NSK        14
#define ELFOSABI_AROS       15
#define ELFOSABI_FENIXOS    16
#define ELFOSABI_CLOUDABI   17
#define ELFOSABI_OPENVOS    18
#define ELFOSABI_ARM_AEABI  64
#define ELFOSABI_ARM        97
#define ELFOSABI_STANDALONE 255

#define EI_ABIVERSION 8

#define EI_PAD 7

// Values for e_type
#define ET_NONE   0 // No file type
#define ET_REL    1 // Relocatable
#define ET_EXEC   2 // Executable
#define ET_DYN    3 // Shared Object
#define ET_CORE   4 // Core
#define ET_LOOS   0xfe00 // OS-specific range start
#define ET_HIOS   0xfeff // OS-specific range end
#define ET_LOPROC 0xff00 // Processor-specific range start
#define ET_HIPROC 0xffff // Processor-specific range end

// Values for e_machine
#define EM_NONE 0x00     // No specific instruction set
#define EM_WE32100 0x01     // AT&T WE 32100
#define EM_SPARC 0x02     // SPARC
#define EM_X86 0x03     // x86
#define EM_M68K 0x04     // Motorola 68000 (M68k)
#define EM_M88K 0x05     // Motorola 88000 (M88k)
#define EM_MCU 0x06     // Intel MCU
#define EM_I80860 0x07     // Intel 80860
#define EM_MIPS 0x08     // MIPS
#define EM_S370 0x09     // IBM System/370
#define EM_MIPS_RS3000 0x0A     // MIPS RS3000 Little-endian
#define EM_HP_PA_RISC 0x0F     // Hewlett-Packard PA-RISC
#define EM_I80960 0x13     // Intel 80960
#define EM_POWERPC 0x14     // PowerPC
#define EM_POWERPC64 0x15     // PowerPC (64-bit)
#define EM_S390 0x16     // S390, including S390x
#define EM_SPU_SPC 0x17     // IBM SPU/SPC
#define EM_NEC_V800 0x24     // NEC V800
#define EM_FR20 0x25     // Fujitsu FR20
#define EM_TRW_RH32 0x26     // TRW RH-32
#define EM_MRCE 0x27     // Motorola RCE
#define EM_ARM 0x28     // Arm (up to Armv7/AArch32)
#define EM_DA 0x29     // Digital Alpha
#define EM_SUPERH 0x2A     // SuperH
#define EM_SPARC9 0x2B     // SPARC Version 9
#define EM_TRICORE 0x2C     // Siemens TriCore embedded processor
#define EM_ARC 0x2D     // Argonaut RISC Core
#define EM_H8300 0x2E     // Hitachi H8/300
#define EM_H8300H 0x2F     // Hitachi H8/300H
#define EM_H8S 0x30     // Hitachi H8S
#define EM_H8500 0x31     // Hitachi H8/500
#define EM_IA64 0x32     // IA-64
#define EM_MIPSX 0x33     // Stanford MIPS-X
#define EM_COLDFIRE 0x34     // Motorola ColdFire
#define EM_M68HC12 0x35     // Motorola M68HC12
#define EM_MMA 0x36     // Fujitsu MMA Multimedia Accelerator
#define EM_PCP 0x37     // Siemens PCP
#define EM_NCPU 0x38     // Sony nCPU embedded RISC processor
#define EM_NDR1 0x39     // Denso NDR1 microprocessor
#define EM_STARCORE 0x3A     // Motorola Star*Core processor
#define EM_ME16 0x3B     // Toyota ME16 processor
#define EM_ST100 0x3C     // STMicroelectronics ST100 processor
#define EM_TINYJ 0x3D     // Advanced Logic Corp. TinyJ embedded processor family
#define EM_X86_64 0x3E     // AMD x86-64
#define EM_DSP 0x3F     // Sony DSP Processor
#define EM_PDP10 0x40     // Digital Equipment Corp. PDP-10
#define EM_PDP11 0x41     // Digital Equipment Corp. PDP-11
#define EM_FX66 0x42     // Siemens FX66 microcontroller
#define EM_ST9 0x43     // STMicroelectronics ST9+ 8/16 bit microcontroller
#define EM_ST7 0x44     // STMicroelectronics ST7 8-bit microcontroller
#define EM_MHC16 0x45     // Motorola MC68HC16 Microcontroller
#define EM_MHC11 0x46     // Motorola MC68HC11 Microcontroller
#define EM_MHC08 0x47     // Motorola MC68HC08 Microcontroller
#define EM_MHC05 0x48     // Motorola MC68HC05 Microcontroller
#define EM_SVX 0x49     // Silicon Graphics SVx
#define EM_ST19 0x4A     // STMicroelectronics ST19 8-bit microcontroller
#define EM_VAX 0x4B     // Digital VAX
#define EM_AXIS 0x4C     // Axis Communications 32-bit embedded processor
#define EM_INFINEON 0x4D     // Infineon Technologies 32-bit embedded processor
#define EM_ELEMENT 0x4E     // Element 14 64-bit DSP Processor
#define EM_LSI 0x4F     // LSI Logic 16-bit DSP Processor
#define EM_TMS320 0x8C     // TMS320C6000 Family
#define EM_MCST 0xAF     // MCST Elbrus e2k
#define EM_ARM64 0xB7     // Arm 64-bits (Armv8/AArch64)
#define EM_Z80 0xDC     // Zilog Z80
#define EM_RISCV 0xF3     // RISC-V
#define EM_BPF 0xF7     // Berkeley Packet Filter
#define EM_WDC 0x101     // WDC 65C816
#define EM_LOONGARCH 0x102     // LoongArch

// Values for e_version
#define EV_NONE 0 // Invalid
#define EV_CURRENT // Current


// Program Header
typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_Phdr;

// Values for p_type
#define PT_NULL         0
#define PT_LOAD         1
#define PT_DYNAMIC      2
#define PT_INTERP       3
#define PT_NOTE         4
#define PT_SHLIB        5
#define PT_PHDR         6
#define PT_TLS          7
#define PT_NUM          8
#define PT_LOOS         0x60000000
#define PT_GNU_EH_FRAME 0x6474e550
#define PT_GNU_STACK    0x6474e551
#define PT_GNU_RELRO    0x6474e552
#define PT_GNU_PROPERTY 0x6474e553
#define PT_GNU_SFRAME   0x6474e554
#define PT_LOSUNW       0x6ffffffa
#define PT_SUNWBSS      0x6ffffffa
#define PT_SUNWSTACK    0x6ffffffb
#define PT_HISUNW       0x6fffffff
#define PT_HIOS         0x6fffffff
#define PT_LOPROC       0x70000000
#define PT_HIPROC       0x7fffffff

// Flags for p_flags
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4


// Section Header
typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} Elf64_Shdr;

// Section indices
#define SHN_UNDEF       0      // Undefined section
#define SHN_LORESERVE   0xff00 // Start of reserved indices
#define SHN_LOPROC      0xff00 // Start of processor-specific
#define SHN_HIPROC      0xff1f // End of processor-specific
#define SHN_LOOS        0xff20 // Start of OS-specific
#define SHN_HIOS        0xff3f // End of OS-specific
#define SHN_ABS         0xfff1 // Associated symbol is absolute
#define SHN_COMMON      0xfff2 // Associated symbol is common
#define SHN_XINDEX      0xffff // Index is in extra table.
#define SHN_HIRESERVE   0xffff // End of reserved indices

// Values for sh_type
#define SHT_NULL          0
#define SHT_PROGBITS      1
#define SHT_SYMTAB        2
#define SHT_STRTAB        3
#define SHT_RELA          4
#define SHT_HASH          5
#define SHT_DYNAMIC       6
#define SHT_NOTE          7
#define SHT_NOBITS        8
#define SHT_REL           9
#define SHT_SHLIB         10
#define SHT_DYNSYM        11
#define SHT_INIT_ARRAY    14
#define SHT_FINI_ARRAY    15
#define SHT_PREINIT_ARRAY 16
#define SHT_GROUP         17
#define SHT_SYMTAB_SHNDX  18
#define SHT_LOOS          0x60000000
#define SHT_HIOS          0x6fffffff
#define SHT_LOPROC        0x70000000
#define SHT_HIPROC        0x7fffffff
#define SHT_LOUSER        0x80000000
#define SHT_HIUSER        0xffffffff

// Values for sh_flags
#define SHF_WRITE            0x1
#define SHF_ALLOC            0x2
#define SHF_EXECINSTR        0x4
#define SHF_MERGE            0x10
#define SHF_STRINGS          0x20
#define SHF_INFO_LINK        0x40
#define SHF_LINK_ORDER       0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP            0x200
#define SHF_TLS              0x400
#define SHF_COMPRESSED       0x800
#define SHF_EXCLUDE          0x80000000
#define SHF_MASKOS           0x0ff00000
#define SHF_MASKPROC         0xf0000000


// Symbols table
typedef struct {
    uint32_t st_name;
    unsigned char st_info;
    unsigned char st_other;
    uint16_t st_shndx;
    uint64_t st_value;
    uint64_t st_size;
} Elf64_Sym;


// Symbol binding
#define ELF64_ST_BIND(i)   ((i)>>4)
#define STB_LOCAL   0
#define STB_GLOBAL  1
#define STB_WEAK    2
#define STB_LOOS    10
#define STB_HIOS    12
#define STB_LOPROC  13
#define STB_HIPROC  15

// Symbol type
#define ELF64_ST_TYPE(i)   ((i)&0xf)
#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_COMMON  5
#define STT_TLS     6
#define STT_LOOS    10
#define STT_HIOS    12
#define STT_LOPROC  13
#define STT_HIPROC  15

#define ELF64_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

#define ELF64_ST_VISIBILITY(o) ((o)&0x3)
#define STV_DEFAULT     0
#define STV_INTERNAL    1
#define STV_HIDDEN      2
#define STV_PROTECTED   3


// Functions

const char *get_class(uint8_t e_class);
const char *get_osabi(uint8_t e_osabi);
const char *get_etype(uint16_t e_type);
const char *get_machine(uint16_t e_machine);
const char *get_ptype(uint32_t p_type);
const char *get_stype(uint32_t sh_type);
const char *get_sflags(uint64_t sh_flags);
const char *get_sym_type(uint64_t st_info);
const char* get_sym_bind(uint64_t st_info);
const char *get_sym_vis(uint64_t st_info);
const char *get_sym_ndx(uint64_t st_shndx);
const char *get_string(char *file, uint32_t sh_name);
