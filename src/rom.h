#ifndef ROM_H
#define ROM_H
#include <stdio.h>

#include "./credits.h"
#include "./initial_sram.h"
#include "./text.h"

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
#else
#endif
