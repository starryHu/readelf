#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "elf.h"
#include "elf_func.h"

#define ELF_CLAS_IDX 4
#define ELF32 1
#define ELF64 2

char *read_file_data(char *file)
{
    if(file == NULL)
        return NULL;
    int fd = -1;
    int file_len = -1;
    char *file_buf = NULL;
    if((fd = open(file, O_RDONLY)) < 0){
        printf("Open file %s failed, %s.\n", file, strerror(errno));
        return NULL;
    }
    if((file_len = lseek(fd, 0, SEEK_END)) < 0){
        printf("Seek to end of file failed, %s.\n", strerror(errno));
        goto exit;
    }
    if((lseek(fd, 0, SEEK_SET)) < 0){
        printf("Seek to start of file failed, %s.\n", strerror(errno));
        goto exit;
    }
    if((file_buf = malloc(file_len)) < 0){
        printf("Malloc failed, %s.\n", strerror(errno));
        goto exit;
    }
    if((read(fd, file_buf, file_len)) < 0){
        printf("Read data from file failed, %s.\n", strerror(errno));
        free(file_buf);
        file_buf = NULL;
        goto exit;
    }
exit:
    if(fd > 0)
        close(fd);
    return file_buf;
}

int main(int argc, char *argv[])
{
    int ret = -1;
    if(argc < 2){
        printf("Usage:./a.out file.\n");
        return ret;
    }
    char *file_buf = NULL;
    if((file_buf = read_file_data(argv[1])) == NULL){
        printf("Read data from %s failed.\n", argv[1]);
        goto exit;
    }
    printf("ELF Header:\n");
    if(file_buf[ELF_CLAS_IDX] == ELF32){
        Elf32_Ehdr *elf32_ehdr = NULL;
        elf32_ehdr = (Elf32_Ehdr *)file_buf;
        print_elf32_elf_header(elf32_ehdr);
        print_elf32_shdr(file_buf, elf32_ehdr->e_shoff, elf32_ehdr->e_shnum, elf32_ehdr->e_shstrndx);
        print_elf32_sym(file_buf, elf32_ehdr);

    }else if(file_buf[ELF_CLAS_IDX] == ELF64){
        Elf64_Ehdr *elf64_ehdr = NULL;
        elf64_ehdr = (Elf64_Ehdr *)file_buf;
        print_elf64_elf_header(elf64_ehdr);
        print_elf64_shdr(file_buf, elf64_ehdr->e_shoff, elf64_ehdr->e_shnum, elf64_ehdr->e_shstrndx);
        print_elf64_sym(file_buf, elf64_ehdr);
    }
exit:
    if(file_buf){
        free(file_buf);
        file_buf = NULL;
    }
    return ret;
}
