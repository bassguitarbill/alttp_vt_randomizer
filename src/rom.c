#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./rom.h"

#define ROM_SIZE 2097152

void rom_write(Rom* rom, int offset, unsigned char data[], int data_size) {
  fseek(rom->tmp_file, offset, SEEK_SET);
  fwrite(data, 1, data_size, rom->tmp_file);
}

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
  unsigned char* source_buffer = malloc(source_size);
  fread(source_buffer, source_size, 1, source_file);
  fclose(source_file);

  rom->tmp_file = tmpfile();

  fwrite(source_buffer, source_size, 1, rom->tmp_file);
  free(source_buffer);

  rom->credits = construct_credits();
  rom->text = construct_text();
  text_remove_unwanted(rom->text);
  rom->initial_sram = construct_initial_sram();
  return rom;
}

void rom_set_heart_colors(Rom* rom, char* color) {
  char byte;
  char file_byte;
  if (strcmp(color, "blue") == 0) {
    byte = 0x2C;
    file_byte = 0x0D;
  } else if (strcmp(color, "green") == 0) {
    byte = 0x3C;
    file_byte = 0x19;
  } else if (strcmp(color, "yellow") == 0) {
    byte = 0x28;
    file_byte = 0x09;
  } else {
    byte = 0x24;
    file_byte = 0x05;
  }

  rom_write(rom, 0x6FA1E, &byte, 1);
  rom_write(rom, 0x6FA20, &byte, 1);
  rom_write(rom, 0x6FA22, &byte, 1);
  rom_write(rom, 0x6FA24, &byte, 1);
  rom_write(rom, 0x6FA26, &byte, 1);
  rom_write(rom, 0x6FA28, &byte, 1);
  rom_write(rom, 0x6FA2A, &byte, 1);
  rom_write(rom, 0x6FA2C, &byte, 1);
  rom_write(rom, 0x6FA2E, &byte, 1);
  rom_write(rom, 0x6FA30, &byte, 1);

  rom_write(rom, 0x65561, &file_byte, 1);
}

void rom_correct_checksum(Rom* rom) {
  fseek(rom->tmp_file, 0, SEEK_SET);
  unsigned int sum = 0x1FE;
  unsigned char buffer[1024];

  for (int i = 0; i < ROM_SIZE; i += 1024) {
    fread(buffer, 1, 1024, rom->tmp_file);
    for (int j = 0; j < 1024; ++j) {
      if ((j + i) >= 0x7FDC && (j + i) < 0x7FE0) {
	continue;
      }
      sum += buffer[j];
    }
  }

  int checksum = sum & 0xFFFF;
  char checksum_data[2];
  checksum_data[1] = (checksum & 0xFF00) >> 8;
  checksum_data[0] = checksum & 0xFF;

  int inverse = checksum ^ 0xFFFF;
  char inverse_data[2];
  inverse_data[1] = (inverse & 0xFF00) >> 8;
  inverse_data[0] = inverse & 0xFF;

  rom_write(rom, 0x7FDC, inverse_data, 2);
  rom_write(rom, 0x7FDE, checksum_data, 2);
}

void rom_save (Rom* rom, char* output_path) {
  fseek(rom->tmp_file, 0L, SEEK_END);
  long output_size = ftell(rom->tmp_file);
  rewind(rom->tmp_file);
  unsigned char* output_buffer = malloc(output_size);
  fread(output_buffer, output_size, 1, rom->tmp_file);
  fclose(rom->tmp_file);

  FILE* output_file = fopen(output_path, "wb");
  fwrite(output_buffer, output_size, 1, output_file);
  fclose(output_file);
  free(output_buffer);
}
