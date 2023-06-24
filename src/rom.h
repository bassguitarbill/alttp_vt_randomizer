#ifndef ROM_H
#define ROM_H
#include <stdio.h>
#include <stdbool.h>

#include "./credits.h"
#include "./initial_sram.h"
#include "./text.h"

#define ROM_SIZE 2097152
#define ROM_BUILD "2023-03-15"
#define ROM_HASH "29b5b39ee65ddfa94638ba2e020a66cd"

/*
 * Wrapper for ROM file
 */
typedef struct {
  FILE* tmp_file;
  Credits* credits;
  Text* text;
  InitialSram* initial_sram;
} Rom;

Rom* construct_rom(char* source_location);
bool rom_check_md5(Rom* rom);
void rom_resize(Rom* rom);
void rom_set_heart_colors(Rom* rom, char* color);
void rom_set_heart_beep_speed(Rom* rom, char* setting);
void rom_set_quick_swap(Rom* rom, char* enable);
void rom_correct_checksum(Rom* rom);
void rom_save(Rom* rom, char* output_path);
#else
#endif
