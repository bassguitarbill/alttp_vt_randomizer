#include "./rom.h"

int main() {
  Rom* rom = construct_rom("base.sfc");
  rom_set_heart_colors(rom, "green");
  rom_correct_checksum(rom);
  rom_save(rom, "output.sfc");
  return 0;
}
