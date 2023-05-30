#include <SDL2/SDL.h>
#include <getopt.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "cd-rom.h"

cd_controller_t cd_controller;

extern fox32_vm_t vm;

void new_cd(const char *filename) {
    if (cd_controller.cd.size) remove_cd();
    printf("mounting %s as CD-ROM\n", filename);
    cd_controller.cd.file = fopen(filename, "rb");
    if (!cd_controller.cd.file) {
        fprintf(stderr, "couldn't open file\n");
        exit(1);
    }
    fseek(cd_controller.cd.file, 0, SEEK_END);
    cd_controller.cd.size = ftell(cd_controller.cd.file);
    rewind(cd_controller.cd.file);
}

void insert_cd(cd_t cd) {
    if (cd_controller.cd.size > 0) remove_cd();
    printf("mounting CD-ROM\n");
    cd_controller.cd = cd;
}

void remove_cd() {
    printf("unmounting CD-ROM\n");
    fclose(cd_controller.cd.file);
    cd_controller.cd.size = 0;
}

uint64_t get_cd_size() {
    return cd_controller.cd.size;
}

void set_cd_sector(uint64_t sector) {
    fseek(cd_controller.cd.file, sector * 512, 0);
}

size_t read_cd_into_memory() {
    if (cd_controller.buffer_pointer >= FOX32_MEMORY_RAM) {
        printf("invalid CD-ROM DMA address: 0x%p", (void *) cd_controller.buffer_pointer);
        return 0;
    }
    return fread(&vm.memory_ram[cd_controller.buffer_pointer], 1, 512, cd_controller.cd.file);
}
