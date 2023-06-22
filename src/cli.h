#ifndef CLI_H
#define CLI_H

#include <stdbool.h>
#include "./rom.h"

typedef struct {
  char* input_file;
  char* output_directory;
  bool spoiler;
  char* heartbeep;
  char* heartcolor;
  bool skip_md5;
  bool tournament;
  int bulk;
  char* sprite;
  bool no_rom;
  bool no_music;
  char* menu_speed;
  char* goal;
  char* state;
  char* weapons;
  char* glitches;
  int crystals_ganon;
  int crystals_tower;
  char* item_placement;
  char* dungeon_items;
  char* accessibility;
  char* hints;
  char* item_pool;
  char* item_functionality;
  char* quickswap;
} CliArguments;
void handle(int argc, char** argv);

#endif
