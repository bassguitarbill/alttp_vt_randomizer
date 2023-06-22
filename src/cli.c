#include <stdlib.h>
#include "./cli.h"

void cli_randomize(CliArguments cli_args) {
  Rom* rom = construct_rom(cli_args.input_file);

  rom_set_heart_colors(rom, cli_args.heartcolor);
  rom_set_heart_beep_speed(rom, cli_args.heartbeep);
  rom_set_quick_swap(rom, cli_args.quickswap);

  rom_correct_checksum(rom);

  char output_filename[1024];
  char* output_filename_pattern = "%s/alttpr_%s_%s_%s_%%s.%%s";
  if (snprintf(output_filename, sizeof(output_filename), output_filename_pattern,
	   cli_args.output_directory,
	   cli_args.glitches,
	   cli_args.state,
	       cli_args.goal) >= sizeof(output_filename)) {
    printf("Your filename is just too big!\n");
    exit(1);
  }

  char* hash = "hash"; // TODO make this a real hash
  char* rom_suffix = "sfc";

  char rom_filename[1024];
  if (snprintf(rom_filename,
	       sizeof(rom_filename),
	       output_filename,
	       hash,
	       rom_suffix) >= sizeof(rom_filename)) {
    printf("Your ROM filename is just too big!\n");
    exit(1);
  }

  printf("Saving ROM to \"%s\"\n", rom_filename);
  rom_save(rom, rom_filename);
}

void handle(int argc, char** argv) {
  CliArguments cli_args;
  cli_args.input_file = "base.sfc";
  cli_args.output_directory = "./";
  cli_args.heartbeep = "half";
  cli_args.heartcolor = "blue";
  cli_args.quickswap = "true";
  cli_args.glitches = "false";
  cli_args.state = "standard";
  cli_args.goal = "ganon";
  cli_randomize(cli_args);
}

