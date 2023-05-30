#include <SDL2/SDL.h>
#include <getopt.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "bus.h"
#include "buttons.h"
#include "cd-rom.h"
#include "cpu.h"
#include "framebuffer.h"
#include "memory-card.h"
#include "serial.h"

bool bus_requests_exit = false;

extern time_t rtc_time;
extern uint32_t rtc_uptime;

extern fox32_vm_t vm;
extern uint16_t button_bitmap;
extern cd_controller_t cd_controller;
extern memory_card_controller_t memory_card_controller;

int bus_io_read(void *user, uint32_t *value, uint32_t port) {
    (void) user;
    switch (port) {
#ifndef WINDOWS
        case 0x00000000: { // serial port
            *value = serial_get();
            break;
        };
#endif

        case 0x80000000 ... 0x8000031F: { // overlay port
            uint8_t overlay_number = port & 0x000000FF;
            uint8_t setting = (port & 0x0000FF00) >> 8;
            switch (setting) {
                case 0x00: {
                    // overlay position
                    uint32_t x = overlay_get(overlay_number)->x;
                    uint32_t y = overlay_get(overlay_number)->y;
                    *value = (y << 16) | x;
                    break;
                };
                case 0x01: {
                    // overlay size
                    uint32_t width = overlay_get(overlay_number)->width;
                    uint32_t height = overlay_get(overlay_number)->height;
                    *value = (height << 16) | width;
                    break;
                };
                case 0x02: {
                    // overlay framebuffer pointer
                    *value = overlay_get(overlay_number)->pointer;
                    break;
                };
                case 0x03: {
                    // overlay enable status
                    *value = overlay_get(overlay_number)->enabled ? 1 : 0;
                    break;
                };
            }

            break;
        };

        case 0x80000400: { // button port
            *value = button_bitmap;

            break;
        };

        case 0x80000700 ... 0x80000707: { // RTC port
            uint8_t setting = port & 0x000000FF;
            struct tm *now = localtime(&rtc_time);
            switch (setting) {
                case 0x00: { // year
                    *value = now->tm_year + 1900;
                    break;
                }
                case 0x01: { // month
                    *value = now->tm_mon + 1;
                    break;
                }
                case 0x02: { // day
                    *value = now->tm_mday;
                    break;
                }
                case 0x03: { // hour
                    *value = now->tm_hour;
                    break;
                }
                case 0x04: { // minute
                    *value = now->tm_min;
                    break;
                }
                case 0x05: { // second
                    *value = now->tm_sec;
                    break;
                }
                case 0x06: { // ms since startup
                    *value = rtc_uptime;
                    break;
                }
                case 0x07: { // daylight savings time active
                    *value = now->tm_isdst;
                    break;
                }
            }

            break;
        }

        case 0x80001000 ... 0x80002000: { // CD-ROM controller port
            uint8_t operation = (port & 0x0000F000) >> 8;
            switch (operation) {
                case 0x10: {
                    // current insert state of CD-ROM
                    // size will be zero if disc isn't inserted
                    *value = get_cd_size();
                    break;
                };
                case 0x20: {
                    // current buffer pointer
                    *value = cd_controller.buffer_pointer;
                    break;
                };
            }

            break;
        };

        case 0x80006000 ... 0x80007001: { // memory card controller port
            size_t id = port & 0xFF;
            uint8_t operation = (port & 0x0000F000) >> 8;
            switch (operation) {
                case 0x60: {
                    // current insert state of specified memory card id
                    // size will be zero if card isn't inserted
                    *value = get_memory_card_size(id);
                    break;
                };
                case 0x70: {
                    // current buffer pointer
                    *value = memory_card_controller.buffer_pointer;
                    break;
                };
            }

            break;
        };
    }

    return 0;
}

int bus_io_write(void *user, uint32_t value, uint32_t port) {
    (void) user;
    switch (port) {
        case 0x00000000: { // serial port
            serial_put(value);
            break;
        };

        case 0x80000000 ... 0x8000031F: { // overlay port
            uint8_t overlay_number = (port & 0x000000FF);
            uint8_t setting = (port & 0x0000FF00) >> 8;
            switch (setting) {
                case 0x00: {
                    // overlay position
                    uint32_t x = value & 0x0000FFFF;
                    uint32_t y = (value & 0xFFFF0000) >> 16;
                    overlay_get(overlay_number)->x = x;
                    overlay_get(overlay_number)->y = y;
                    break;
                };
                case 0x01: {
                    // overlay size
                    uint32_t width = value & 0x0000FFFF;
                    uint32_t height = (value & 0xFFFF0000) >> 16;
                    overlay_get(overlay_number)->width = width;
                    overlay_get(overlay_number)->height = height;
                    break;
                };
                case 0x02: {
                    // overlay framebuffer pointer
                    overlay_get(overlay_number)->pointer = value;
                    break;
                };
                case 0x03: {
                    // overlay enable status
                    overlay_get(overlay_number)->enabled = value != 0;
                    break;
                };
            }

            break;
        };

        case 0x80001000 ... 0x80005000: { // CD-ROM controller port
            uint8_t operation = (port & 0x0000F000) >> 8;
            switch (operation) {
                case 0x10: {
                    // no-op
                    break;
                };
                case 0x20: {
                    // set the buffer pointer
                    cd_controller.buffer_pointer = value;
                    break;
                };
                case 0x30: {
                    // read specified disk sector into memory
                    set_cd_sector(value);
                    read_cd_into_memory();
                    break;
                };
                case 0x40: {
                    // no-op
                    break;
                };
                case 0x50: {
                    // remove CD-ROM
                    remove_cd();
                    break;
                };
            }

            break;
        };

        case 0x80006000 ... 0x8000A003: { // memory card controller port
            size_t id = port & 0xFF;
            uint8_t operation = (port & 0x0000F000) >> 8;
            switch (operation) {
                case 0x60: {
                    // no-op
                    break;
                };
                case 0x70: {
                    // set the buffer pointer
                    memory_card_controller.buffer_pointer = value;
                    break;
                };
                case 0x80: {
                    // read specified memory card sector into memory
                    set_memory_card_sector(id, value);
                    read_memory_card_into_memory(id);
                    break;
                };
                case 0x90: {
                    // write specified memory card sector from memory
                    set_memory_card_sector(id, value);
                    write_memory_card_from_memory(id);
                    break;
                };
                case 0xA0: {
                    // remove specified memory card
                    remove_memory_card(id);
                    break;
                };
            }

            break;
        };


        case 0x80010000: { // power control port
            if (value == 0) {
                bus_requests_exit = true;
            }
        };
    }

    return 0;
}

void drop_file(char *filename) {
    new_cd(filename);
}
