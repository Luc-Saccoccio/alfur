#include <stdio.h>
#include <string.h>

#include "elf.h"

const char *get_class(uint8_t e_class) {
    static char s[16];
    memset(s, 0, 16);

    switch (e_class) {
        case ELFCLASSNONE: return "INVALID";
        case ELFCLASS32:   return "ELF32";
        case ELFCLASS64:   return "ELF64";

        default:
            snprintf(s, 16, "UNKNOWN (%d)", e_class);
            return s;
    }
}

const char *get_osabi(uint8_t e_osabi) {
    static char s[16];
    memset(s, 0, 16);

    switch (e_osabi) {
        case ELFOSABI_SYSV:       return "System V";
        case ELFOSABI_HPUX:       return "HP-UX";
        case ELFOSABI_NETBSD:     return "NetBSD";
        case ELFOSABI_LINUX:      return "Linux";
        case ELFOSABI_HURD:       return "GNU Hurd";
        case ELFOSABI_SOLARIS:    return "Solaris";
        case ELFOSABI_AIX:        return "AIX";
        case ELFOSABI_IRIX:       return "Irix";
        case ELFOSABI_FREEBSD:    return "FreeBSD";
        case ELFOSABI_TRU64:      return "Tru64";
        case ELFOSABI_MODESTO:    return "Novell Modesto";
        case ELFOSABI_OPENBSD:    return "OpenBSD";
        case ELFOSABI_OPENVMS:    return "OpenVMS";
        case ELFOSABI_NSK:        return "Hewlett-Packard Non-Stop Kernel";
        case ELFOSABI_AROS:       return "Amiga Research OS";
        case ELFOSABI_FENIXOS:    return "FenixOS";
        case ELFOSABI_CLOUDABI:   return "Nuxi CloudABI";
        case ELFOSABI_OPENVOS:    return "Stratus Technologies OpenVOS";
        case ELFOSABI_ARM_AEABI:  return "ARM EABI";
        case ELFOSABI_ARM:        return "ARM";
        case ELFOSABI_STANDALONE: return "Standalone application";

        default:
            snprintf(s, 16, "Unknown (%d)", e_osabi);
            return s;
    }
}

const char *get_etype(uint16_t e_type) {
    static char s[16];
    memset(s, 0, 16);

    if ((e_type >= ET_LOOS) && (e_type) <= ET_HIOS) {
        snprintf(s, 16, "OS+%#x", e_type);
        return s;
    }

    if ((e_type >= ET_LOPROC) && (e_type) <= ET_HIPROC) {
        snprintf(s, 16, "PROC+%#x)", e_type);
        return s;
    }

    switch (e_type) {
        case ET_NONE: return "NONE";
        case ET_REL:  return "REL";
        case ET_EXEC: return "EXEC";
        case ET_DYN:  return "DYN";
        case ET_CORE: return "CORE";

        default:
            snprintf(s, 16, "UNK+%#x", e_type);
            return s;
    }
}

const char *get_machine(uint16_t e_machine) {
    static char s[16];
    memset(s, 0, 16);

    switch (e_machine) {
        case EM_NONE:         return "No specific instruction set";
        case EM_WE32100:      return "AT&T WE 32100";
        case EM_SPARC:        return "SPARC";
        case EM_X86:          return "x86";
        case EM_M68K:         return "Motorola 68000 (M68k)";
        case EM_M88K:         return "Motorola 88000 (M88k)";
        case EM_MCU:          return "Intel MCU";
        case EM_I80860:       return "Intel 80860";
        case EM_MIPS:         return "MIPS";
        case EM_S370:         return "IBM System/370";
        case EM_MIPS_RS3000:  return "MIPS RS3000 Little-endian";
        case EM_HP_PA_RISC:   return "Hewlett-Packard PA-RISC";
        case EM_I80960:       return "Intel 80960";
        case EM_POWERPC:      return "PowerPC";
        case EM_POWERPC64:    return "PowerPC (64-bit)";
        case EM_S390:         return "S390, including S390x";
        case EM_SPU_SPC:      return "IBM SPU/SPC";
        case EM_NEC_V800:     return "NEC V800";
        case EM_FR20:         return "Fujitsu FR20";
        case EM_TRW_RH32:     return "TRW RH-32";
        case EM_MRCE:         return "Motorola RCE";
        case EM_ARM:          return "Arm (up to Armv7/AArch32)";
        case EM_DA:           return "Digital Alpha";
        case EM_SUPERH:       return "SuperH";
        case EM_SPARC9:       return "SPARC Version 9";
        case EM_TRICORE:      return "Siemens TriCore embedded processor";
        case EM_ARC:          return "Argonaut RISC Core";
        case EM_H8300:        return "Hitachi H8/300";
        case EM_H8300H:       return "Hitachi H8/300H";
        case EM_H8S:          return "Hitachi H8S";
        case EM_H8500:        return "Hitachi H8/500";
        case EM_IA64:         return "IA-64";
        case EM_MIPSX:        return "Stanford MIPS-X";
        case EM_COLDFIRE:     return "Motorola ColdFire";
        case EM_M68HC12:      return "Motorola M68HC12";
        case EM_MMA:          return "Fujitsu MMA Multimedia Accelerator";
        case EM_PCP:          return "Siemens PCP";
        case EM_NCPU:         return "Sony nCPU embedded RISC processor";
        case EM_NDR1:         return "Denso NDR1 microprocessor";
        case EM_STARCORE:     return "Motorola Star*Core processor";
        case EM_ME16:         return "Toyota ME16 processor";
        case EM_ST100:        return "STMicroelectronics ST100 processor";
        case EM_TINYJ:        return "Advanced Logic Corp. TinyJ embedded processor family";
        case EM_X86_64:       return "AMD x86-64";
        case EM_DSP:          return "Sony DSP Processor";
        case EM_PDP10:        return "Digital Equipment Corp. PDP-10";
        case EM_PDP11:        return "Digital Equipment Corp. PDP-11";
        case EM_FX66:         return "Siemens FX66 microcontroller";
        case EM_ST9:          return "STMicroelectronics ST9+ 8/16 bit microcontroller";
        case EM_ST7:          return "STMicroelectronics ST7 8-bit microcontroller";
        case EM_MHC16:        return "Motorola MC68HC16 Microcontroller";
        case EM_MHC11:        return "Motorola MC68HC11 Microcontroller";
        case EM_MHC08:        return "Motorola MC68HC08 Microcontroller";
        case EM_MHC05:        return "Motorola MC68HC05 Microcontroller";
        case EM_SVX:          return "Silicon Graphics SVx";
        case EM_ST19:         return "STMicroelectronics ST19 8-bit microcontroller";
        case EM_VAX:          return "Digital VAX";
        case EM_AXIS:         return "Axis Communications 32-bit embedded processor";
        case EM_INFINEON:     return "Infineon Technologies 32-bit embedded processor";
        case EM_ELEMENT:      return "Element 14 64-bit DSP Processor";
        case EM_LSI:          return "LSI Logic 16-bit DSP Processor";
        case EM_TMS320:       return "TMS320C6000 Family";
        case EM_MCST:         return "MCST Elbrus e2k";
        case EM_ARM64:        return "Arm 64-bits (Armv8/AArch64)";
        case EM_Z80:          return "Zilog Z80";
        case EM_RISCV:        return "RISC-V";
        case EM_BPF:          return "Berkeley Packet Filter";
        case EM_WDC:          return "WDC 65C816";
        case EM_LOONGARCH:    return "LoongArch";

        default:
            snprintf(s, 16, "Unknown (%#x)", e_machine);
            return s;

    }
}

const char *get_ptype(uint32_t p_type) {
    static char s[16];
    memset(s, 0, 16);

    if ((p_type >= PT_LOOS) && (p_type <= PT_HIOS)) {
            switch (p_type) {
                case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
                case PT_GNU_STACK:    return "GNU_STACK";
                case PT_GNU_RELRO:    return "GNU_RELRO";
                case PT_GNU_PROPERTY: return "GNU_PROPERTY";
                case PT_GNU_SFRAME:   return "GNU_SFRAME";
                case PT_SUNWBSS:      return "SUNWBSS";
                case PT_SUNWSTACK:    return "SUNWSTACK";
                default:
                    if (p_type >= PT_LOSUNW)
                        snprintf(s, 16, "SUN+%#x", p_type);
                    else
                        snprintf(s, 16, "OS+%#x", p_type);
                    return s;
        }
    }

    if ((p_type >= PT_LOPROC) && (p_type <= PT_HIPROC)) {
        snprintf(s, 16, "PROC+%#x", p_type);
        return s;
    }

    switch (p_type) {
        case PT_NULL:    return "NULL";
        case PT_LOAD:    return "LOAD";
        case PT_DYNAMIC: return "DYNAMIC";
        case PT_INTERP:  return "INTERP";
        case PT_NOTE:    return "NOTE";
        case PT_SHLIB:   return "SHLIB";
        case PT_PHDR:    return "PHDR";
        case PT_TLS:     return "TLS";
        case PT_NUM:     return "NUM";
        default:
            snprintf(s, 16, "UNK+%#x", p_type);
            return s;
    }
}

const char *get_interp(Elf64_Phdr *elf_phead, char* elf_image) {
    static char s[1024] = "INTERP: ";
    s[8] = 0;
    strcpy(s + 8, elf_image + elf_phead->p_offset);
    return s;
}

const char *get_stype(uint32_t sh_type) {
    static char s[16];
    memset(s, 0, 16);

    if ((sh_type >= SHT_LOOS) && (sh_type <= SHT_HIOS)) {
        snprintf(s, 16, "OS+%#x", sh_type);
        return s;
    }

    if ((sh_type >= SHT_LOPROC) && (sh_type <= SHT_HIPROC)) {
        snprintf(s, 16, "PROC+%#x", sh_type);
        return s;
    }

    if ((sh_type >= SHT_LOUSER) && (sh_type <= SHT_HIUSER)) {
        snprintf(s, 16, "USER+%#x", sh_type);
        return s;
    }

    switch (sh_type) {
        case SHT_NULL:          return "NULL";
        case SHT_PROGBITS:      return "PROGBITS";
        case SHT_SYMTAB:        return "SYMTAB";
        case SHT_STRTAB:        return "STRTAB";
        case SHT_RELA:          return "RELA";
        case SHT_HASH:          return "HASH";
        case SHT_DYNAMIC:       return "DYNAMIC";
        case SHT_NOTE:          return "NOTE";
        case SHT_NOBITS:        return "NOBITS";
        case SHT_REL:           return "REL";
        case SHT_SHLIB:         return "SHLIB";
        case SHT_DYNSYM:        return "DYNSYM";
        case SHT_INIT_ARRAY:    return "INIT_ARRAY";
        case SHT_FINI_ARRAY:    return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY: return "PREINIT_ARRAY";
        case SHT_GROUP:         return "GROUP";
        case SHT_SYMTAB_SHNDX:  return "SYMTAB_SHNDX";

        default:
            snprintf(s, 16, "UNK+%#x", sh_type);
            return s;
    }
}

const char *get_sflags(uint64_t sh_flags) {
    static char s[16];
    memset(s, ' ', 15);

    const struct {
        uint64_t f;
        char c;
    } flags[14] =
        {
            { SHF_WRITE, 'W' },
            { SHF_ALLOC, 'A' },
            { SHF_EXECINSTR, 'X' },
            { SHF_MERGE, 'M' },
            { SHF_STRINGS, 'S' },
            { SHF_INFO_LINK, 'I' },
            { SHF_LINK_ORDER, 'L' },
            { SHF_OS_NONCONFORMING, 'O' },
            { SHF_GROUP, 'G' },
            { SHF_TLS, 'T' },
            { SHF_COMPRESSED, 'C' },
            { SHF_EXCLUDE, 'E' },
            { SHF_MASKOS, 'o' },
            { SHF_MASKPROC, 'p' }
        };

    for (int i = 0; i < 14; i++) {
        if (sh_flags & flags[i].f) {
            s[i] = flags[i].c;
            sh_flags &= ~flags[i].f;
        }
    }

    // If there's till flags, mark as unkown
    if (sh_flags)
        s[14] = 'u';

    s[15] = 0;
    return s;
}

const char *get_sym_type(uint64_t st_info) {
    uint64_t type = ELF64_ST_TYPE(st_info);
    static char s[16];
    memset(s, 0, 16);

    switch (type) {
        case STT_NOTYPE:  return "NOTYPE";
        case STT_OBJECT:  return "OBJECT";
        case STT_FUNC:    return "FUNC";
        case STT_SECTION: return "SECTION";
        case STT_FILE:    return "FILE";
        case STT_COMMON:  return "COMMON";
        case STT_TLS:     return "TLS";
        default:
            snprintf(s, 16, "UNK+%ld", type);
            return s;

    }
}

const char* get_sym_bind(uint64_t st_info) {
    uint64_t bind = ELF64_ST_BIND(st_info);
    static char s[16];
    memset(s, 0, 16);

    switch (bind) {
        case STB_LOCAL:   return "LOCAL";
        case STB_GLOBAL:  return "GLOBAL";
        case STB_WEAK:    return "WEAK";
        default:
            snprintf(s, 16, "UNK+%ld", bind);
            return s;
    }
}

const char *get_sym_vis(uint64_t st_info) {
    uint64_t vis = ELF64_ST_VISIBILITY(st_info);
    static char s[16];

    switch (vis) {
        case STV_DEFAULT:   return "DEFAULT";
        case STV_INTERNAL:  return "INTERNAL";
        case STV_HIDDEN:    return "HIDDEN";
        case STV_PROTECTED: return "PROTECTED";
        default:
            snprintf(s, 16, "UNK+%ld", vis);
            return s;
    }
}

const char *get_sym_ndx(uint64_t st_shndx) {
    static char s[16];
    memset(s, 0, 16);

    switch (st_shndx) {
        case SHN_UNDEF:  return "UND";
        case SHN_ABS:    return "ABS";
        case SHN_COMMON: return "COM";
        default:
            snprintf(s, 16, "%3ld", st_shndx);
            return s;
    }
}

const char *get_string(char *table, uint32_t sh_name) {
    return table + sh_name;
}
