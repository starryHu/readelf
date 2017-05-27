#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "elf.h"

#define SYM_BIT_X86 64
#define SYM_BIT_ARM 32

static void print_type(int type)
{
    char *tmp = NULL;
    switch(type){
        case ET_NONE: tmp = "None"; break;
        case ET_REL:  tmp = "Relocatable file"; break;
        case ET_EXEC: tmp = "Executable file"; break;
        case ET_DYN:  tmp = "Shared object file"; break;
        case ET_CORE: tmp = "Core file"; break;
        default: tmp = "Uknown";
    }
    printf(" %-35s%s\n", "type:", tmp);
}

#define PRINT_ELF_EHDR_ELE(class) \
    do{ \
        char *fmt = NULL; \
        print_type((int)elf_ehdr->e_type); \
        if(class == SYM_BIT_ARM) \
            fmt = " %-35s0x%x\n"; \
        else if(class == SYM_BIT_X86) \
            fmt = " %-35s0x%lx\n"; \
        printf(fmt, "Start of program headers:", elf_ehdr->e_phoff); \
        printf(fmt, "Start of section headers:", elf_ehdr->e_shoff); \
        fmt = " %-35s%d\n"; \
        printf(fmt, "Size of this header:", elf_ehdr->e_ehsize); \
        printf(fmt, "Size of program headers:", elf_ehdr->e_phentsize); \
        printf(fmt, "Number of program headers:", elf_ehdr->e_phnum); \
        printf(fmt, "Size of section headers:", elf_ehdr->e_shentsize); \
        printf(fmt, "Number of section headers:", elf_ehdr->e_shnum); \
        printf(fmt, "Section header string table index:", elf_ehdr->e_shstrndx); \
    }while(0)

void print_elf32_elf_header(Elf32_Ehdr *elf_ehdr)
{
        printf(" %-35sELF32\n", "Class:");
        PRINT_ELF_EHDR_ELE(SYM_BIT_ARM);
}

void print_elf64_elf_header(Elf64_Ehdr *elf_ehdr)
{
        printf(" %-35sELF64\n", "Class:");
        PRINT_ELF_EHDR_ELE(SYM_BIT_X86);
}

#define PRINT_ELF_SHDR_ELE(class) \
    do{ \
        char *tmp = NULL; \
        printf("%-20s", base_addr + elf_shdr->sh_name); \
        switch(elf_shdr->sh_type){ \
            case SHT_NULL:      tmp = "NULL"; break; \
            case SHT_PROGBITS:  tmp = "PROGBITS"; break; \
            case SHT_SYMTAB:    tmp = "SYMTAB"; break;  \
            case SHT_STRTAB:    tmp = "STRTAB"; break; \
            case SHT_RELA:      tmp = "RELA"; break; \
            case SHT_HASH:      tmp = "HASH"; break; \
            case SHT_DYNAMIC:   tmp = "DYNAMIC"; break; \
            case SHT_NOTE:      tmp = "NOTE"; break; \
            case SHT_NOBITS:    tmp = "NOBITS"; break; \
            case SHT_REL:       tmp = "REL"; break; \
            case SHT_SHLIB:     tmp = "SHLIB"; break; \
            case SHT_DYNSYM:    tmp = "DYNSYM"; break; \
            case SHT_INIT_ARRAY:    tmp = "INIT_ARRY"; break; \
            case SHT_FINI_ARRAY:    tmp = "FINI_ARRAY"; break; \
            case SHT_PREINIT_ARRAY: tmp = "PREINIT_ARRAY"; break;  \
            case SHT_GROUP:         tmp = "GROUP"; break; \
            case SHT_SYMTAB_SHNDX:  tmp = "SYMTAB_SHNDX"; break; \
            case SHT_NUM:           tmp = "NUM"; break; \
            case SHT_LOOS:          tmp = "LOOS"; break; \
            case SHT_GNU_LIBLIST:   tmp = "GNU_LIBLIST"; break;  \
            case SHT_CHECKSUM:      tmp = "CHECKSUM"; break; \
            case SHT_SUNW_move:     tmp = "SUNW_MOVE"; break; \
            case SHT_SUNW_COMDAT:   tmp = "SUNW_COMDAT"; break; \
            case SHT_SUNW_syminfo:  tmp = "SUNW_syminfo"; break; \
            case SHT_GNU_verdef:    tmp = "GNU_verdef"; break; \
            case SHT_GNU_verneed:   tmp = "GNU_VERNEED"; break; \
            case SHT_GNU_versym:    tmp = "GNU_VERSYM"; break; \
            case SHT_LOPROC:        tmp = "LOPROC"; break; \
            case SHT_HIPROC:        tmp = "HIPROC"; break; \
            case SHT_LOUSER:        tmp = "LOUSER"; break; \
            case SHT_HIUSER:        tmp = "HIUSER"; break; \
            case SHT_ARM_EXIDX:     tmp = "ARM_EXIDX"; break; \
            case SHT_ARM_PREEMPTMAP:tmp = "ARM_PREEMPTMAP"; break; \
            case SHT_ARM_ATTRIBUTES:tmp = "ARM_ATTRIBUTES"; break; \
            default:            tmp = "Unknown"; \
        } \
        printf("%-18s", tmp); \
        char *fmt = NULL; \
        if(class == SYM_BIT_ARM) \
            fmt = "%08x "; \
        else if(class == SYM_BIT_X86) \
            fmt = "%016lx "; \
        printf(fmt, elf_shdr->sh_addr); \
        if(class == SYM_BIT_ARM) \
            fmt = "%06x "; \
        else if(class == SYM_BIT_X86) \
            fmt = "%08lx "; \
        printf(fmt, elf_shdr->sh_offset); \
        if(class == SYM_BIT_ARM) \
            fmt = "%06x "; \
        else if(class == SYM_BIT_X86) \
            fmt = "%016lx "; \
        printf(fmt, elf_shdr->sh_size); \
        printf("\n"); \
    }while(0)


static void print_elf32_shdr_detail(char *base_addr, Elf32_Shdr *elf_shdr)
{
    PRINT_ELF_SHDR_ELE(SYM_BIT_ARM);
}

static void print_elf64_shdr_detail(char *base_addr, Elf64_Shdr *elf_shdr)
{
    PRINT_ELF_SHDR_ELE(SYM_BIT_X86);
}

#define PRINT_ELF_SHDR(cls) PRINT_ELF_SHDR_(cls)
#define PRINT_ELF_SHDR_(class) \
    do{ \
        printf("-------------------------------------------------------\n"); \
        printf("Section Header:\n"); \
        Elf##class##_Shdr *elf_shdr = NULL; \
        Elf##class##_Shdr *elf_shdr_str_tab = NULL; \
        int i = 0; \
        elf_shdr = (Elf##class##_Shdr *)(base_addr + sh_off); \
        elf_shdr_str_tab = (Elf##class##_Shdr *)(base_addr + sh_off + ((Elf##class##_Ehdr *)base_addr)->e_shentsize*sh_str_idx); \
        char *sh_str_base = base_addr + elf_shdr_str_tab->sh_offset; \
        char *fmt = NULL; \
        if(class == SYM_BIT_ARM) \
            fmt = "NO   %-20s%-18s%-8s %-6s %-6s\n"; \
        else if(class == SYM_BIT_X86) \
            fmt = "NO   %-20s%-18s%-16s %-8s %-16s\n"; \
        printf(fmt, "Name", "Type", "Addr", "Offset", "Size"); \
        for(i = 0; i < sh_num; i++){ \
            printf("[%2d] ", i); \
            print_elf##class##_shdr_detail(sh_str_base, elf_shdr); \
            elf_shdr++; \
        } \
    }while(0)

void print_elf32_shdr(char *base_addr, unsigned long sh_off, unsigned long sh_num, unsigned long sh_str_idx)
{
    PRINT_ELF_SHDR(SYM_BIT_ARM);
}

void print_elf64_shdr(char *base_addr, unsigned long sh_off, unsigned long sh_num, unsigned long sh_str_idx)
{
    PRINT_ELF_SHDR(SYM_BIT_X86);
}

#define PRINT_ELF_SYM_ELE(cls) PRINT_ELF_SYM_ELE_(cls) 
#define PRINT_ELF_SYM_ELE_(class) \
    do{ \
        char *fmt = NULL; \
        if(class == SYM_BIT_ARM) \
            fmt = "%08x "; \
        else if(class == SYM_BIT_X86) \
            fmt = "%016lx "; \
        printf(fmt, elf_sym->st_value); \
        printf("%7lld ", (unsigned long long)elf_sym->st_size); \
        char *tmp = NULL; \
        switch(ELF##class##_ST_TYPE(elf_sym->st_info)){ \
            case STT_NOTYPE:    tmp = "NOTYPE"; break; \
            case STT_OBJECT:    tmp = "OBJECT"; break; \
            case STT_FUNC:      tmp = "FUNC"; break; \
            case STT_SECTION:   tmp = "SECTION"; break; \
            case STT_FILE:      tmp = "FILE"; break; \
            case STT_COMMON:    tmp = "COMMON"; break; \
            case STT_TLS:       tmp = "TLS"; break; \
            case STT_NUM:       tmp = "NUM"; break; \
            case STT_LOOS:      tmp = "LOOS"; break; \
            case STT_HIOS:      tmp = "HIOS"; break; \
            case STT_LOPROC:    tmp = "LOPROC"; break; \
            case STT_HIPROC:    tmp = "HIPROC"; break; \
            default: tmp = "Unknown"; \
        } \
        printf("%-9s ", tmp); \
        switch(ELF##class##_ST_BIND(elf_sym->st_info)){ \
            case STB_LOCAL:     tmp = "LOCAL"; break;  \
            case STB_GLOBAL:    tmp = "GLOBAL"; break; \
            case STB_WEAK:      tmp = "WEAK"; break; \
            case STB_NUM:       tmp = "NUM"; break; \
            case STB_LOOS:      tmp = "LOOS"; break; \
            case STB_HIOS:      tmp = "HIOS"; break; \
            case STB_LOPROC:    tmp = "LOPROC"; break; \
            case STB_HIPROC:    tmp = "HIPROC"; break; \
            default: tmp = "Unknown"; \
        } \
        printf("%-10s ", tmp); \
        printf("%s", addr_base + elf_sym->st_name); \
        printf("\n"); \
    }while(0)

static void print_elf32_symbol_detail(char *addr_base, Elf32_Sym *elf_sym)
{
    PRINT_ELF_SYM_ELE(SYM_BIT_ARM);
}

static void print_elf64_symbol_detail(char *addr_base, Elf64_Sym *elf_sym)
{
    PRINT_ELF_SYM_ELE(SYM_BIT_X86);
}

#define PRINT_ELF_SYM(cls) PRINT_ELF_SYM_(cls)
#define PRINT_ELF_SYM_(class) \
    do{ \
        printf("-------------------------------------------------------\n"); \
        printf("%s\n", ind_str); \
        Elf##class##_Shdr *elf_shdr = NULL; \
        Elf##class##_Shdr *elf_shdr_str_tab = NULL; \
        Elf##class##_Shdr *elf_shdr_sym_tab = NULL; \
        int sym_tab_idx = 0; \
        Elf##class##_Shdr *elf_shdr_sym_str_tab = NULL; \
        int sym_str_tab_idx = 0; \
        int i = 0; \
        elf_shdr = (Elf##class##_Shdr *)(base_addr + elf_ehdr->e_shoff); \
        elf_shdr_str_tab = (Elf##class##_Shdr *)(base_addr + elf_ehdr->e_shoff + ((Elf##class##_Ehdr *)base_addr)->e_shentsize*elf_ehdr->e_shstrndx); \
        char *sh_str_base = base_addr + elf_shdr_str_tab->sh_offset; \
        for(i = 0; i < elf_ehdr->e_shnum; i++){ \
            if(strncmp(sh_str_base+elf_shdr->sh_name, tab_name, 7) == 0) \
                sym_tab_idx = i; \
            if(strncmp(sh_str_base+elf_shdr->sh_name, strtab_name, 7) == 0) \
                sym_str_tab_idx = i; \
            elf_shdr++; \
        } \
        if(sym_str_tab_idx >= elf_ehdr->e_shnum) \
            printf("There is no strtab in file.\n"); \
        if(sym_tab_idx >= elf_ehdr->e_shnum) \
            printf("There is no symtab in file.\n"); \
        elf_shdr_sym_tab  = (Elf##class##_Shdr *)(base_addr + elf_ehdr->e_shoff + ((Elf##class##_Ehdr *)base_addr)->e_shentsize*sym_tab_idx); \
        Elf##class##_Sym *elf_sym = NULL; \
        int sym_num = 0; \
        elf_sym = (Elf##class##_Sym *)(base_addr + elf_shdr_sym_tab->sh_offset);  \
        sym_num = elf_shdr_sym_tab->sh_size / sizeof(Elf##class##_Sym); \
        elf_shdr_sym_str_tab = (Elf##class##_Shdr *)(base_addr + elf_ehdr->e_shoff + ((Elf##class##_Ehdr *)base_addr)->e_shentsize*sym_str_tab_idx); \
        char *sym_str_base = base_addr + elf_shdr_sym_str_tab->sh_offset; \
        char *fmt = NULL; \
        if(class == SYM_BIT_ARM) \
            fmt = "%7s %8s %7s %-9s %-10s %s\n"; \
        else if(class == SYM_BIT_X86) \
            fmt = "%7s %16s %7s %-9s %-10s %s\n"; \
        printf(fmt, "Num:", "Value", "Size", "Type", "Bind", "Name"); \
        for(i = 0; i < sym_num; i++){ \
            printf("%6d: ", i); \
            print_elf##class##_symbol_detail(sym_str_base, elf_sym); \
            elf_sym++; \
        } \
    }while(0)

void print_elf32_sym(char *base_addr, Elf32_Ehdr *elf_ehdr)
{
    char *tab_name = ".symtab";
    char *strtab_name = ".strtab";
    char *ind_str = "Symbol table:";
    PRINT_ELF_SYM(SYM_BIT_ARM);
}

void print_elf64_sym(char *base_addr, Elf64_Ehdr *elf_ehdr)
{
    char *tab_name = ".symtab";
    char *strtab_name = ".strtab";
    char *ind_str = "Symbol table:";
    PRINT_ELF_SYM(SYM_BIT_X86);
}

void print_elf32_dynsym(char *base_addr, Elf32_Ehdr *elf_ehdr)
{
    char *tab_name = ".dynsym";
    char *strtab_name = ".dynstr";
    char *ind_str = "Dynamic Symbol table:";
    PRINT_ELF_SYM(SYM_BIT_ARM);
}

void print_elf64_dynsym(char *base_addr, Elf64_Ehdr *elf_ehdr)
{
    char *tab_name = ".dynsym";
    char *strtab_name = ".dynstr";
    char *ind_str = "Dynamic Symbol table:";
    PRINT_ELF_SYM(SYM_BIT_X86);
}
