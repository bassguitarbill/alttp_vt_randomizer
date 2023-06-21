#include <stdio.h>
#include <stdlib.h>
#include "./rom.h"

/*
 * Create a new wrapper
 *
 * @param char* source_location location of source ROM to edit
 */
Rom* construct_rom(char* source_location) {
  Rom* rom = malloc(sizeof(Rom));
  FILE *source_file = fopen(source_location, "rb");
  if (source_file == NULL) {
    printf("Source ROM not readable\n");
    exit(1);
  }

  fseek(source_file, 0L, SEEK_END);
  long source_size = ftell(source_file);
  rewind(source_file);
  void* source_buffer = malloc(source_size);
  fread(source_buffer, source_size, 1, source_file);
  fclose(source_file);

  char* temp_file_name = tmpnam(NULL);
  rom->tmp_file = fopen(temp_file_name, "wb");
  if (rom->tmp_file == NULL) {
    printf("could not make temp file");
  }

  fwrite(source_buffer, source_size, 1, rom->tmp_file);
  printf("Successfully wrote %d bytes to %s\n", source_size, temp_file_name);

  rom->credits = construct_credits();
  rom->text = construct_text();
  text_remove_unwanted(rom->text);
  rom->initial_sram = construct_initial_sram();
  return rom;
}
