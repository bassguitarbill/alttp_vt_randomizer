#include "./rom.h"

int main() {
  Rom* rom = construct_rom("base.sfc");

  rom_set_heart_colors(rom, "green");
  rom_set_heart_beep_speed(rom, "double");
  rom_set_quick_swap(rom, true);

  rom_correct_checksum(rom);
  rom_save(rom, "output.sfc");

  return 0;
}
