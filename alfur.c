#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "elf.h"

// General structure for manipulating ELF

typedef struct {
    Elf64_Ehdr *elf_head; // ELF Header
    char *elf_shead; // Start of section headers
    char *elf_phead; // Start of program headers
    Elf64_Shdr* shstr_table_header;
    char *shstr_table;
    char *elf_image;
} Elf64_data;

Elf64_Shdr *get_section(Elf64_data *data, uint64_t index) {
    return (Elf64_Shdr*)(data->elf_shead + (index * data->elf_head->e_shentsize));
}

void usage(void) {
    fprintf(stderr, "Usage: alfur <file>\n");
    exit(1);
}

void error(const char *message) {
    fprintf(stderr, message, strerror(errno));
    exit(1);
}

void test_valid(char *elf_image) {
    uint8_t *magic = (uint8_t*)elf_image;
    if (*magic != ELFMAG0 || (strncmp(magic+1, "ELF", 3) != 0)) {
        fprintf(stderr, "The file is not a valid ELF file!\n");
        exit(1);
    }
}

void display_header(Elf64_data* file, char* elf_path) {
    char encoding[16];
    Elf64_Ehdr *elf_head = file->elf_head;

    if (elf_head->e_ident[EI_DATA] == ELFDATA2LSB)
        strncpy(encoding, "Little Endian", 16);
    else if (elf_head->e_ident[EI_DATA] == ELFDATA2MSB)
        strncpy(encoding, "Big Endian", 16);

    fprintf(stdout, "=== Alfur ===\n");
    fprintf(stdout, "ELF statistics for %s, %s, ELF version %d\n",
            elf_path, encoding, elf_head->e_ident[EI_VERSION]);
    fprintf(stdout, "  Class:                      %s\n", get_class(elf_head->e_ident[EI_CLASS]));
    fprintf(stdout, "  Version:                    %d\n", elf_head->e_ident[EI_VERSION]);
    fprintf(stdout, "  OS/ABI:                     %s\n", get_osabi(elf_head->e_ident[EI_OSABI]));
    fprintf(stdout, "  ABI Version:                %d\n", elf_head->e_ident[EI_ABIVERSION]);
    fprintf(stdout, "  Type:                       %s\n", get_etype(elf_head->e_type));
    fprintf(stdout, "  Machine:                    %s\n", get_machine(elf_head->e_machine));
    fprintf(stdout, "  Version:                    0x%x\n", elf_head->e_version);
    fprintf(stdout, "  Entry Point Access:         0x%lx\n", elf_head->e_entry);
    fprintf(stdout, "  Start of program headers:   %lu\n", elf_head->e_phoff);
    fprintf(stdout, "  Start of section headers:   %lu\n", elf_head->e_shoff);
    fprintf(stdout, "  Flags:                      %d\n", elf_head->e_flags);
    fprintf(stdout, "  Size of this header:        %d\n", elf_head->e_ehsize);
    fprintf(stdout, "  Size of program headers:    %d\n", elf_head->e_phentsize);
    fprintf(stdout, "  Number of program headers:  %d\n", elf_head->e_phnum);
    fprintf(stdout, "  Size of section headers:    %d\n", elf_head->e_shentsize);
    fprintf(stdout, "  Number of section headers:  %d\n", elf_head->e_shnum);
    fprintf(stdout, "  Section header table index: %d\n", elf_head->e_shstrndx);

    file->elf_phead = file->elf_image + file->elf_head->e_phoff;
    file->elf_shead = file->elf_image + file->elf_head->e_shoff;
    file->shstr_table_header = get_section(file, file->elf_head->e_shstrndx);
    file->shstr_table = file->elf_image + file->shstr_table_header->sh_offset;
}

void display_programs(Elf64_data *file) {
    fprintf(stdout, "\n== Program Headers ==\n");

    if (file->elf_head->e_phnum == 0) {
        fprintf(stderr, "\nNo progam header\n");
        return;
    }
    Elf64_Phdr *elf_phead = (Elf64_Phdr*)file->elf_phead;

    for (int i = 0; i < file->elf_head->e_phnum; i++, elf_phead++) {
        fprintf(stdout, "\n* %s\n",
                elf_phead->p_type ^ PT_INTERP
                    ? get_ptype(elf_phead->p_type)
                    : get_interp(elf_phead, file->elf_image));
        fprintf(stdout, "            Offset 0x%16.16lx   0x%16.16lx Virtual Address\n", elf_phead->p_offset, elf_phead->p_vaddr);
        fprintf(stdout, "  Physical Address 0x%16.16lx   0x%16.16lx File Size\n", elf_phead->p_paddr, elf_phead->p_filesz);
        fprintf(stdout, "       Memory Size 0x%16.16lx   %c%c%c   0x%-10lx Flags & Align\n", elf_phead->p_memsz,
                (elf_phead->p_flags & PF_R ? 'R' : ' '),
                (elf_phead->p_flags & PF_W ? 'W' : ' '),
                (elf_phead->p_flags & PF_X ? 'X' : ' '),
                elf_phead->p_align);
    }

}

void display_sections(Elf64_data *file) {
    fprintf(stdout, "\n== Section Headers ==\n\n");

    if (file->elf_head->e_shnum == 0) {
        fprintf(stderr, "No section header\n");
        return;
    }

    Elf64_Shdr *elf_shead = (Elf64_Shdr*)file->elf_shead;

    for (int i = 0; i < file->elf_head->e_shnum; i++, elf_shead++) {
        fprintf(stdout, "  [%2d] %s (%s)\n", i,
                get_string(file->shstr_table, elf_shead->sh_name), get_stype(elf_shead->sh_type));
        fprintf(stdout, "      Address 0x%16.16lx   0x%16.16lx Offset\n", elf_shead->sh_addr, elf_shead->sh_offset);
        fprintf(stdout, "         Size 0x%16.16lx   0x%16.16lx EntSize\n", elf_shead->sh_size, elf_shead->sh_entsize);
        fprintf(stdout, "        Flags %18s   %-9d %-8d Link & Info\n", get_sflags(elf_shead->sh_flags), elf_shead->sh_link, elf_shead->sh_info);
        fprintf(stdout, "        Align %ld\n", elf_shead->sh_addralign);
    }
}

void display_symbols(Elf64_Shdr *section, Elf64_data *file) {
    fprintf(stdout, "\n= Symbol table %s =\n\n",
            get_string(file->shstr_table, section->sh_name));

    if (section->sh_entsize == 0) {
        fprintf(stderr, "Symbol table %s has a sh_entsize of 0\n",
            get_string(file->shstr_table, section->sh_name));
        return;
    }

    uint64_t sym_num = section->sh_size / section->sh_entsize;
    Elf64_Sym *sym = (Elf64_Sym*)(file->elf_image + section->sh_offset);
    Elf64_Shdr *sym_names_table_header = get_section(file, section->sh_link);
    char *sym_names_table = file->elf_image + sym_names_table_header->sh_offset;

    fprintf(stdout, "  Num:  Value            Size Type    Bind   Visibility Ndx Name\n");
    for (int i = 0; i < sym_num; i++, sym++) {
        fprintf(stdout, "  {%5d}: %16.16lx %4ld", i, sym->st_value, sym->st_size);
        fprintf(stdout, " %-7s %-6s %-9s", get_sym_type(sym->st_info), get_sym_bind(sym->st_info),
                get_sym_vis(sym->st_other));
        fprintf(stdout, "  %s %s\n", get_sym_ndx(sym->st_shndx),
                ELF64_ST_TYPE(sym->st_info) & STT_SECTION
                    ? get_string(file->shstr_table, get_section(file, sym->st_shndx)->sh_name)
                    : get_string(sym_names_table, sym->st_name));
    }

}

void display_strings(Elf64_Shdr *section, Elf64_data *file) {
    fprintf(stdout, "\n= String table '%s' =\n\n", get_string(file->shstr_table, section->sh_name));
    uint8_t newline = 0;
    char *data, *start, *end;
    data = start = file->elf_image + section->sh_offset;
    end = start + section->sh_size;

    while (data < end) {
        while (!isprint(*data))
            if (++data >= end)
                break;
        if (data < end) {
            size_t maxlen = end - data;

            if (newline) {
                fprintf(stdout, "                 ");
                newline = 0;
            }

            fprintf(stdout, "   [|%8tx|]  ", data - start);
            if (maxlen > 0) {
                char c = 0;
                while (maxlen) {
                    c = *data++;

                    if (c == 0)
                        break;

                    if (c == '\n') {
                        fprintf(stdout, "\\n\n");
                        if (*data != 0)
                            newline = 1;
                        break;
                    }

                    if (iscntrl(c))
                        fprintf(stdout, "^%c", c + 0x40);
                    else if (isprint(c))
                        fputc(c, stdout);
                    else
                        fprintf(stdout, "%.1s", data - 1);
                }

                if (c != '\n')
                    fputc('\n', stdout);
            }
        }
    }
}

void display_rel(Elf64_Shdr *section, Elf64_data *file) {
    fprintf(stdout, "\n= Relocation table '%s' =\n\n", get_string(file->shstr_table, section->sh_name));

    if (section->sh_entsize == 0) {
        fprintf(stderr, "Relocations table %s has a sh_entsize of 0\n",
            get_string(file->shstr_table, section->sh_name));
        return;
    }

    Elf64_Shdr *symtab_header = get_section(file, section->sh_link);
    char *symtab = file->elf_image + symtab_header->sh_offset;
    Elf64_Shdr *sym_names_table_header = get_section(file, symtab_header->sh_link);

    char *sym_names_table = file->elf_image + sym_names_table_header->sh_offset;

    uint64_t relo_num = section->sh_size / section->sh_entsize;
    Elf64_Rel *entry = (Elf64_Rel*)(file->elf_image + section->sh_offset);
    Elf64_Sym *sym;

    fputs("Offset        Info          Type  Symbol Value     Name\n", stdout);
    for (int i = 0; i < relo_num; i++, entry++) {
        sym = (Elf64_Sym*)(sym_names_table + symtab_header->sh_entsize * ELF64_R_SYM(entry->r_info));
        fprintf(stdout, "%12lx  %12lx %5ld %16lx %s",
                entry->r_offset, entry->r_info, ELF64_R_TYPE(entry->r_info),
                sym->st_value, get_string(sym_names_table, sym->st_name));
    }
}

void display_rela(Elf64_Shdr *section, Elf64_data *file) {
    fprintf(stdout, "\n= Relocation table '%s' =\n\n", get_string(file->shstr_table, section->sh_name));

    if (section->sh_entsize == 0) {
        fprintf(stderr, "Relocations table %s has a sh_entsize of 0\n",
            get_string(file->shstr_table, section->sh_name));
        return;
    }

    Elf64_Shdr *symtab_header = get_section(file, section->sh_link);
    char *symtab = file->elf_image + symtab_header->sh_offset;
    Elf64_Shdr *sym_names_table_header = get_section(file, symtab_header->sh_link);

    char *sym_names_table = file->elf_image + sym_names_table_header->sh_offset;

    uint64_t relo_num = section->sh_size / section->sh_entsize;
    Elf64_Rela *entry = (Elf64_Rela*)(file->elf_image + section->sh_offset);
    Elf64_Sym *sym;

    fputs("Offset        Info          Type  Symbol Value     Name ; Addend\n", stdout);
    for (int i = 0; i < relo_num; i++, entry++) {
        sym = (Elf64_Sym*)(symtab + symtab_header->sh_entsize * ELF64_R_SYM(entry->r_info));
        fprintf(stdout, "%12.12lx  %12.12lx %5ld  %16.16lx %s ; %ld\n",
                entry->r_offset, entry->r_info, ELF64_R_TYPE(entry->r_info),
                sym->st_value, get_string(sym_names_table, sym->st_name), entry->r_addend);
    }
}

void display_note(Elf64_Shdr *section, Elf64_data *file) {
    fprintf(stdout, "\n= Note '%s' =\n\n", get_string(file->shstr_table, section->sh_name));
}

void display_section_contents(Elf64_data *file) {
    fprintf(stdout, "\n== Sections contents ==\n");

    if (file->elf_head->e_shnum == 0) {
        fprintf(stderr, "No sections\n");
        return;
    }

    Elf64_Shdr *section = (Elf64_Shdr*)file->elf_shead;

    for (int i = 0; i < file->elf_head->e_shnum; i++, section++) {
        switch (section->sh_type) {
            case SHT_NULL:
            case SHT_NOBITS:
            case SHT_PROGBITS:
                break;
            case SHT_SYMTAB:
            case SHT_DYNSYM:
                display_symbols(section, file);
                break;
            case SHT_STRTAB:
                display_strings(section, file);
                break;
            case SHT_RELA:
                display_rela(section, file);
                break;
            case SHT_REL:
                display_rel(section, file);
                break;
            case SHT_NOTE:
                display_note(section, file);
            default:
                fprintf(stdout, "= TODO %s =\n", get_string(file->shstr_table, section->sh_name));
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        usage();

    Elf64_data *file = calloc(1, sizeof(Elf64_data));
    char elf_path[128];
    int elf_fd;
    struct stat elf_stat;

    // Get filename
    strncpy(elf_path, argv[1], 128);

    // Open file
    if ((elf_fd = open(elf_path, O_RDONLY)) < 0)
        error("Failed opening the file! %s\n");

    // Get file size
    if (stat(elf_path, &elf_stat) < 0)
        error("Failed determining file size! %s\n");

    // Map file in virtual memory
    if ((file->elf_image = mmap(NULL, elf_stat.st_size, PROT_READ, MAP_PRIVATE, elf_fd, 0)) < 0)
        error("Failed to mmap file to memory! %s\n");

    test_valid(file->elf_image);

    file->elf_head = (Elf64_Ehdr*)file->elf_image;

    display_header(file, elf_path);
    display_programs(file);
    display_sections(file);
    display_section_contents(file);

    if (munmap(file->elf_image, elf_stat.st_size) < 0)
        error("Failed unmapping the file! %s\n");

    free(file);
    return 0;
}
