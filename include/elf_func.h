#ifndef _ELF32_H
#define _ELF32_H
void print_elf32_elf_header(Elf32_Ehdr *elf32_ehdr);
void print_elf64_elf_header(Elf64_Ehdr *elf64_ehdr);
void print_elf32_sym(char *base_addr, Elf32_Ehdr *elf32_ehdr);
void print_elf64_sym(char *base_addr, Elf64_Ehdr *elf64_ehdr);
void print_elf32_shdr(char *base_addr, unsigned long sh_off, unsigned long sh_num, unsigned long sh_str_idx);
void print_elf64_shdr(char *base_addr, unsigned long sh_off, unsigned long sh_num, unsigned long sh_str_idx);

#endif
