#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "elf.h"

// General structure for manipulating ELF

typedef struct {
    Elf64_Ehdr* elf_head; // ELF Header
    Elf64_Shdr* elf_shead; // Start of section headers
    Elf64_Shdr* shstr_table_header;
    Elf64_Phdr* elf_phead; // Start of program headers
    char* elf_image;
    char *shstr_table;
} Elf64_data;


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

    file->shstr_table_header = (Elf64_Shdr*)(file->elf_image + file->elf_head->e_shoff
        + (file->elf_head->e_shstrndx * file->elf_head->e_shentsize));
    file->shstr_table = file->elf_image + file->shstr_table_header->sh_offset;
}

void display_programs(Elf64_data *file) {
    fprintf(stdout, "\n== Program Headers ==\n");

    if (file->elf_head->e_phnum == 0) {
        fprintf(stderr, "\nNo progam header\n");
        return;
    }
    Elf64_Phdr *elf_phead;

    file->elf_phead = (Elf64_Phdr*)(file->elf_image + file->elf_head->e_phoff);
    elf_phead = file->elf_phead;

    for (int i = 0; i < file->elf_head->e_phnum; i++, elf_phead++) {
        fprintf(stdout, "\n* %s\n", get_ptype(elf_phead->p_type));
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

    Elf64_Shdr *elf_shead;
    file->elf_shead = (Elf64_Shdr*)(file->elf_image + file->elf_head->e_shoff);
    elf_shead = file->elf_shead;

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
    Elf64_Shdr *sym_names_table_header = (Elf64_Shdr*)(file->elf_image + file->elf_head->e_shoff
            + (section->sh_link * file->elf_head->e_shentsize));
    char *sym_names_table = file->elf_image + sym_names_table_header->sh_offset;

    fprintf(stdout, "  Num:  Value            Size Type    Bind   Visibility Ndx Name\n");
    for (int i = 0; i < sym_num; i++, sym++) {
        fprintf(stdout, "  {%2d}: %16.16lx %4ld", i, sym->st_value, sym->st_size);
        fprintf(stdout, " %-7s %-6s %-9s", get_sym_type(sym->st_info), get_sym_bind(sym->st_info),
                get_sym_vis(sym->st_other));
        fprintf(stdout, "  %s %s\n", get_sym_ndx(sym->st_shndx),
                get_string(sym_names_table, sym->st_name));
    }

}

void display_strings(Elf64_Shdr *section, Elf64_data *file) {
    fprintf(stdout, "\n= String table contents =\n\n");
}

void display_relocations(Elf64_Shdr *section, Elf64_data *file) {
    fprintf(stdout, "\n= Relocation content =\n\n");
}

void display_section_contents(Elf64_data *file) {
    fprintf(stdout, "\n== Sections contents ==\n\n");

    if (file->elf_head->e_shnum == 0) {
        fprintf(stderr, "No sections\n");
        return;
    }

    Elf64_Shdr *section = file->elf_shead;

    for (int i = 0; i < file->elf_head->e_shnum; i++, section++) {
        switch (section->sh_type) {
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
            case SHT_REL:
                display_relocations(section, file);
                break;
            default:
                fprintf(stdout, "~> TODO (%d)\n", section->sh_type);
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
