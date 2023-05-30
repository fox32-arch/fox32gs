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
#include "memory-card.h"

memory_card_controller_t memory_card_controller;

extern fox32_vm_t vm;

void new_memory_card(const char *filename, size_t id) {
    if (id > 1) { puts("attempting to access memory card with ID > 1"); return; }
    printf("mounting %s as memory card ID %d\n", filename, (int) id);
    memory_card_controller.memory_cards[id].file = fopen(filename, "r+b");
    if (!memory_card_controller.memory_cards[id].file) {
        fprintf(stderr, "couldn't open memory card file\n");
        exit(1);
    }
    fseek(memory_card_controller.memory_cards[id].file, 0, SEEK_END);
    memory_card_controller.memory_cards[id].size = ftell(memory_card_controller.memory_cards[id].file);
    rewind(memory_card_controller.memory_cards[id].file);
}

void insert_memory_card(memory_card_t memory_card, size_t id) {
    if (id > 1) { puts("attempting to access memory card with ID > 1"); return; }
    if (memory_card_controller.memory_cards[id].size > 0) remove_memory_card(id);
    printf("mounting memory card ID %d\n", (int) id);
    memory_card_controller.memory_cards[id] = memory_card;
}

void remove_memory_card(size_t id) {
    if (id > 1) { puts("attempting to access memory card with ID > 1"); return; }
    printf("unmounting memory card ID %d\n", (int) id);
    fclose(memory_card_controller.memory_cards[id].file);
    memory_card_controller.memory_cards[id].size = 0;
}

uint64_t get_memory_card_size(size_t id) {
    if (id > 1) { puts("attempting to access memory card with ID > 1"); return 0; }
    return memory_card_controller.memory_cards[id].size;
}

void set_memory_card_sector(size_t id, uint64_t sector) {
    if (id > 1) { puts("attempting to access memory card with ID > 1"); return; }
    fseek(memory_card_controller.memory_cards[id].file, sector * 512, 0);
}

size_t read_memory_card_into_memory(size_t id) {
    if (id > 1) { puts("attempting to access memory card with ID > 1"); return 0; }
    if (memory_card_controller.buffer_pointer >= FOX32_MEMORY_RAM) {
        printf("invalid memory card DMA address: 0x%p", (void *) memory_card_controller.buffer_pointer);
        return 0;
    }
    return fread(&vm.memory_ram[memory_card_controller.buffer_pointer], 1, 512, memory_card_controller.memory_cards[id].file);
}

size_t write_memory_card_from_memory(size_t id) {
    if (id > 1) { puts("attempting to access memory card with ID > 1"); return 0; }
    if (memory_card_controller.buffer_pointer >= FOX32_MEMORY_RAM) {
        printf("invalid memory card DMA address: 0x%p", (void *) memory_card_controller.buffer_pointer);
        return 0;
    }
    return fwrite(&vm.memory_ram[memory_card_controller.buffer_pointer], 1, 512, memory_card_controller.memory_cards[id].file);
}
