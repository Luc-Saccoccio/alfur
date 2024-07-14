#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "elf.h"

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
    if (magic[0] != ELFMAG0 || (strncmp(magic+1, "ELF", 3) != 0)) {
        fprintf(stderr, "The file is not a valid ELF file!\n");
        exit(1);
    }
}

void display_header(Elf64_Ehdr* elf_head, char* elf_path) {
    char encoding[16];

    if (elf_head->e_ident[EI_DATA] == ELFDATA2LSB)
        strncpy(encoding, "Little Endian", 16);
    else if (elf_head->e_ident[EI_DATA] == ELFDATA2MSB)
        strncpy(encoding, "Big Endian", 16);

    fprintf(stdout, "=== Alfur ===\n");
    fprintf(stdout, "ELF statistics for %s, %s, ELF version %d\n",
            elf_path, encoding, elf_head->e_ident[EI_VERSION]);
    fprintf(stdout, "  Class:                      %s\n", get_class(elf_head));
    fprintf(stdout, "  Version:                    %d\n", elf_head->e_ident[EI_VERSION]);
    fprintf(stdout, "  OS/ABI:                     %s\n", get_osabi(elf_head));
    fprintf(stdout, "  ABI Version:                %d\n", elf_head->e_ident[EI_ABIVERSION]);
    fprintf(stdout, "  Type:                       %s\n", get_type(elf_head));
    fprintf(stdout, "  Machine:                    %s\n", get_machine(elf_head));
    fprintf(stdout, "  Version:                    0x%X\n", elf_head->e_version);
    fprintf(stdout, "  Entry Point Access:         0x%lX\n", elf_head->e_entry);
    fprintf(stdout, "  Start of program headers:   %lu\n", elf_head->e_phoff);
    fprintf(stdout, "  Start of section headers:   %lu\n", elf_head->e_shoff);
    fprintf(stdout, "  Flags:                      %d\n", elf_head->e_flags);
    fprintf(stdout, "  Size of this header:        %d\n", elf_head->e_ehsize);
    fprintf(stdout, "  Size of program headers:    %d\n", elf_head->e_phentsize);
    fprintf(stdout, "  Number of program headers:  %d\n", elf_head->e_phnum);
    fprintf(stdout, "  Size of section headers:    %d\n", elf_head->e_shentsize);
    fprintf(stdout, "  Number of section headers:  %d\n", elf_head->e_shnum);
    fprintf(stdout, "  Section header table index: %d\n", elf_head->e_shstrndx);
}

void display_programs(Elf64_Ehdr *elf_head, char *elf_image) {
    if (elf_head->e_phnum == 0) {
        fprintf(stdout, "No progam header\n");
        return;
    }
    Elf64_Phdr *ptr;
    ptr = (Elf64_Phdr*)(elf_image + elf_head->e_phoff);

    fprintf(stdout, "\nProgram Headers:\n");
            /* "  Type           Offset             VirtAddr           PhysAddr\n"
            "                 FileSiz            MemSiz              Flags  Align"); */

    for (int i = 0; i < elf_head->e_phnum; i++, ptr++) {
        fprintf(stdout, "* Type:             %s\n", get_ptype(ptr));
        fprintf(stdout, "  Flags:            %c%c%c\n",
                (ptr->p_flags & PF_R ? 'R' : ' '),
                (ptr->p_flags & PF_W ? 'W' : ' '),
                (ptr->p_flags & PF_X ? 'X' : ' '));
        fprintf(stdout, "  Offset:           0x%16.16lx\n", (unsigned long)ptr->p_offset);
        fprintf(stdout, "  Virtual Address:  0x%16.16lx\n", (unsigned long)ptr->p_vaddr);
        fprintf(stdout, "  Physical Address: 0x%16.16lx\n", (unsigned long)ptr->p_paddr);
        fprintf(stdout, "  File Size:        0x%16.16lx\n", (unsigned long)ptr->p_filesz);
        fprintf(stdout, "  Memory Size:      0x%16.16lx\n", (unsigned long)ptr->p_memsz);
        fprintf(stdout, "  Align:            %#lx\n", (unsigned long)ptr->p_align);
    }
}

void display_sections(Elf64_Ehdr *elf_head, char *elf_imge) {

}

int main(int argc, char *argv[]) {
    if (argc < 2)
        usage();

    Elf64_Ehdr *elf_head;
    char elf_path[128];
    int elf_fd;
    struct stat elf_stat;
    char *elf_image;

    // Get filename
    strncpy(elf_path, argv[1], 128);

    // Open file
    if ((elf_fd = open(elf_path, O_RDONLY)) < 0)
        error("Failed opening the file! %s\n");

    // Get file size
    if (stat(elf_path, &elf_stat) < 0)
        error("Failed determining file size! %s\n");

    // Map file in virtual memory
    if ((elf_image = mmap(NULL, elf_stat.st_size, PROT_READ, MAP_PRIVATE, elf_fd, 0)) < 0)
        error("Failed to mmap file to memory! %s\n");

    test_valid(elf_image);

    elf_head = (Elf64_Ehdr*)elf_image;

    display_header(elf_head, elf_path);
    display_programs(elf_head, elf_image);

    if (munmap(elf_image, elf_stat.st_size) < 0)
        error("Failed unmapping the file! %s\n");

    return 0;
}
