#pragma once

#include <stdint.h>

typedef struct {
    FILE *file;
    uint64_t size;
} cd_t;

typedef struct {
    cd_t cd;
    size_t buffer_pointer;
} cd_controller_t;

void new_cd(const char *filename);
void insert_cd(cd_t disk);
void remove_cd();
uint64_t get_cd_size();
void set_cd_sector(uint64_t sector);
size_t read_cd_into_memory();
