#pragma once

#include <stdint.h>

typedef struct {
    FILE *file;
    uint64_t size;
} memory_card_t;

typedef struct {
    memory_card_t memory_cards[2];
    size_t buffer_pointer;
} memory_card_controller_t;

void new_memory_card(const char *filename, size_t id);
void insert_memory_card(memory_card_t memory_card, size_t id);
void remove_memory_card(size_t id);
uint64_t get_memory_card_size(size_t id);
void set_memory_card_sector(size_t id, uint64_t sector);
size_t read_memory_card_into_memory(size_t id);
size_t write_memory_card_from_memory(size_t id);
