#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "./cli.h"

void cli_arguments_log(CliArguments cli) {
  printf("input_file: %s\n", cli.input_file);
  printf("output_directory: %s\n", cli.output_directory);
  printf("unrandomized: %d\n", cli.unrandomized);
  printf("spoiler: %d\n", cli.spoiler);
  printf("heartbeep: %s\n", cli.heartbeep);
  printf("heartcolor: %s\n", cli.heartcolor);
  printf("skip_md5: %d\n", cli.skip_md5);
  printf("tournament: %d\n", cli.tournament);
  printf("bulk: %d\n", cli.bulk);
  printf("sprite: %s\n", cli.sprite);
  printf("no_rom: %d\n", cli.no_rom);
  printf("no_music: %d\n", cli.no_music);
  printf("menu_speed: %s\n", cli.menu_speed);
  printf("goal: %s\n", cli.goal);
  printf("state: %s\n", cli.state);
  printf("weapons: %s\n", cli.weapons);
  printf("glitches: %s\n", cli.glitches);
  printf("crystals_ganon: %d\n", cli.crystals_ganon);
  printf("crystals_tower: %d\n", cli.crystals_tower);
  printf("item_placement: %s\n", cli.item_placement);
  printf("dungeon_items: %s\n", cli.dungeon_items);
  printf("accessibility: %s\n", cli.accessibility);
  printf("hints: %s\n", cli.hints);
  printf("item_pool: %s\n", cli.item_pool);
  printf("item_functionality: %s\n", cli.item_functionality);
  printf("quickswap: %s\n", cli.quickswap);
}

CliArguments cli_arguments_default() {
  CliArguments cli;
  cli.unrandomized = false;
  cli.spoiler = false;
  cli.heartbeep = "half";
  cli.heartcolor = "red";
  cli.skip_md5 = false;
  cli.tournament = false;
  cli.bulk = 1;
  cli.sprite = NULL;
  cli.no_rom = false;
  cli.no_music = false;
  cli.menu_speed = "normal";
  cli.goal = "ganon";
  cli.state = "standard";
  cli.weapons = "randomized";
  cli.glitches = "none";
  cli.crystals_ganon = "7";
  cli.crystals_tower = "7";
  cli.item_placement = "basic";
  cli.dungeon_items = "standard";
  cli.accessibility = "item";
  cli.hints = "on";
  cli.item_pool = "normal";
  cli.item_functionality = "normal";
  cli.quickswap = "false";

  return cli;
}

char* cli_output_filename_template(CliArguments cli_args) {
  char* output_filename = malloc(1024);
  char* output_filename_pattern = "%s/alttpr_%s_%s_%s_%%s.%%s";
  if (snprintf(output_filename,
	       1024,
	       output_filename_pattern,
	       cli_args.output_directory,
	       cli_args.glitches,
	       cli_args.state,
	       cli_args.goal) >= 1024) {
    printf("Your filename is just too big!\n");
    exit(1);
  }

  return output_filename;
}

void cli_check_input_file(CliArguments cli_args) {
  if (access(cli_args.input_file, R_OK) != 0) {
    printf("Unable to read input file %s\n", cli_args.input_file);
    exit(1);
  }
}

void cli_check_output_directory(CliArguments cli_args) {
  if (access(cli_args.output_directory, R_OK | W_OK) != 0) {
    printf("Unable to write to output directory %s\n", cli_args.output_directory);
    exit(1);
  }
}

char* cli_get_heart_color(CliArguments cli_args) {
  if (strcmp("random", cli_args.heartcolor) != 0) return cli_args.heartcolor;
  char* colors[] = { "blue", "green", "yellow", "red" };
  return colors[rand() % 4];
}

int get_crystals(char* crystals_input) {
  if (strcmp("random", crystals_input) != 0) return atoi(crystals_input);
  return rand() % 8;
}

char* cli_get_logic(CliArguments cli_args) {
  char* glitches = cli_args.glitches;
  if (streq(glitches, "none")) return "NoGlitches";
  if (streq(glitches, "overworld_glitches")) return "OverworldGlitches";
  if (streq(glitches, "hybrid_major_glitches")) return "HybridMajorGlitches";
  if (streq(glitches, "major_glitches")) return "MajorGlitches";
  if (streq(glitches, "no_logic")) return "NoLogic";
  printf("Invalid argument `glitches`: %s\n", glitches);
  return "NoGlitches";
}

void cli_randomize(CliArguments cli_args) {
  // initialize hasher
  char* hash = "hash"; // TODO make this a real hash
  // check for stringiness of arguments (useless?)
  
  char* filename = cli_output_filename_template(cli_args);
  cli_check_input_file(cli_args);
  cli_check_output_directory(cli_args);

  for (int i = 0; i < cli_args.bulk; i++) {
    // clear Item and Boss caches
    Rom* rom = construct_rom(cli_args.input_file);
    hash = "hash"; // TODO make this a real hash

    if (!cli_args.skip_md5 && !rom_check_md5(rom)) {
      rom_resize(rom);
      // apply patch to rom
    }

    if (!cli_args.skip_md5 && !rom_check_md5(rom)) {
      printf("MD5 check failed ;)\n");
      exit(3);
    }

    rom_set_heart_colors(rom, cli_get_heart_color(cli_args));

    rom_set_heart_colors(rom, cli_args.heartbeep);
    
    rom_set_quick_swap(rom, cli_args.quickswap);

    char rom_filename[1024];
    if (cli_args.unrandomized) {
      rom_correct_checksum(rom);
      // TODO don't use sprintf
      sprintf(rom_filename, "%s/alttp-%s.sfc", cli_args.output_directory, ROM_BUILD);
      rom_save(rom, rom_filename);
      printf("ROM saved: %s\n", rom_filename);
      return; // TODO this exits early if bulk > 1
    }

    int crystals_ganon = get_crystals(cli_args.crystals_ganon);

    int crystals_tower = get_crystals(cli_args.crystals_tower);
    
    char* logic = cli_get_logic(cli_args);

    // Build the world
    // Create a randomizer from that world
    // Randomize it
    // Write the random stuff to the rom

    rom_mute_music(rom, cli_args.no_music);
    rom_set_menu_speed(rom, cli_args.menu_speed);

    char* rom_suffix = "sfc";

    if (snprintf(rom_filename,
		 sizeof(rom_filename),
		 filename,
		 hash,
		 rom_suffix) >= sizeof(rom_filename)) {
      printf("Your ROM filename is just too big!\n");
      exit(1);
    }

    if (!cli_args.no_rom) {
      // do sprite manipulation
      // do tournament stuff

      rom_correct_checksum(rom);
      rom_save(rom, rom_filename);
      printf("ROM saved: %s\n", rom_filename);
    }

    if (!cli_args.spoiler) {
      // write spoiler contents to a file

      // printf("Spoiler saved: %s\n", spoiler_file);
    }
  }

}

void handle(int argc, char** argv) {
  CliArguments cli_args = cli_arguments_default();
  int c;
  static struct option long_options[] = {
    { "unrandomized", no_argument, 0, UNRANDOMIZED },
    { "spoiler", no_argument, 0, SPOILER },
    { "heartbeep", required_argument, 0, HEARTBEEP },
    { "heartcolor", required_argument, 0, HEARTCOLOR },
    { "skip_md5", no_argument, 0, SKIP_MD5 },
    { "tournament", no_argument, 0, TOURNAMENT },
    { "bulk", required_argument, 0, BULK },
    { "sprite", required_argument, 0, SPRITE },
    { "no_rom", no_argument, 0, NO_ROM },
    { "no_music", no_argument, 0, NO_MUSIC },
    { "menu_speed", required_argument, 0, MENU_SPEED },
    { "goal", required_argument, 0, GOAL },
    { "state", required_argument, 0, STATE },
    { "weapons", required_argument, 0, WEAPONS },
    { "glitches", required_argument, 0, GLITCHES },
    { "crystals_ganon", required_argument, 0, CRYSTALS_GANON },
    { "crystals_tower", required_argument, 0, CRYSTALS_TOWER },
    { "item_placement", required_argument, 0, ITEM_PLACEMENT },
    { "dungeon_items", required_argument, 0, DUNGEON_ITEMS },
    { "accessibility", required_argument, 0, ACCESSIBILITY },
    { "hints", required_argument, 0, HINTS },
    { "item_pool", required_argument, 0, ITEM_POOL },
    { "item_functionality", required_argument, 0, ITEM_FUNCTIONALITY },
    { "quickswap", required_argument, 0, QUICKSWAP },
    { 0, 0, 0, 0 },
  };

  while (true) {
    int option_index = 0;
    c = getopt_long(argc, argv, "", long_options, &option_index);
    if (c == -1) break;

    switch (c) {
    case UNRANDOMIZED:
      cli_args.unrandomized = true;
      break;
    case SPOILER:
      cli_args.spoiler = true;
      break;
    case HEARTBEEP:
      cli_args.heartbeep = malloc(1 + strlen(optarg));
      strcpy(cli_args.heartbeep, optarg);
      break;
    case HEARTCOLOR:
      cli_args.heartcolor = malloc(1 + strlen(optarg));
      strcpy(cli_args.heartcolor, optarg);
      break;
    case SKIP_MD5:
      cli_args.skip_md5 = true;
      break;
    case TOURNAMENT:
      cli_args.tournament = true;
      break;
    case BULK:
      // TODO handle non-numeric arguments
      cli_args.bulk = atoi(optarg);
      break;
    case SPRITE:
      cli_args.sprite = malloc(1 + strlen(optarg));
      strcpy(cli_args.sprite, optarg);
      break;
    case NO_ROM:
      cli_args.no_rom = true;
      break;
    case NO_MUSIC:
      cli_args.no_music = true;
      break;
    case MENU_SPEED:
      cli_args.menu_speed = malloc(1 + strlen(optarg));
      strcpy(cli_args.menu_speed, optarg);
      break;
    case GOAL:
      cli_args.goal = malloc(1 + strlen(optarg));
      strcpy(cli_args.goal, optarg);
      break;
    case STATE:
      cli_args.state = malloc(1 + strlen(optarg));
      strcpy(cli_args.state, optarg);
      break;
    case WEAPONS:
      cli_args.weapons = malloc(1 + strlen(optarg));
      strcpy(cli_args.weapons, optarg);
      break;
    case GLITCHES:
      cli_args.glitches = malloc(1 + strlen(optarg));
      strcpy(cli_args.glitches, optarg);
      break;
    case CRYSTALS_GANON:
      cli_args.crystals_ganon = malloc(1 + strlen(optarg));
      strcpy(cli_args.crystals_ganon, optarg);
      break;
    case CRYSTALS_TOWER:
      cli_args.crystals_tower = malloc(1 + strlen(optarg));
      strcpy(cli_args.crystals_tower, optarg);
      break;
    case ITEM_PLACEMENT:
      cli_args.item_placement = malloc(1 + strlen(optarg));
      strcpy(cli_args.item_placement, optarg);
      break;
    case DUNGEON_ITEMS:
      cli_args.dungeon_items = malloc(1 + strlen(optarg));
      strcpy(cli_args.dungeon_items, optarg);
      break;
    case ACCESSIBILITY:
      cli_args.accessibility = malloc(1 + strlen(optarg));
      strcpy(cli_args.accessibility, optarg);
      break;
    case HINTS:
      cli_args.hints = malloc(1 + strlen(optarg));
      strcpy(cli_args.hints, optarg);
      break;
    case ITEM_POOL:
      cli_args.item_pool = malloc(1 + strlen(optarg));
      strcpy(cli_args.item_pool, optarg);
      break;
    case ITEM_FUNCTIONALITY:
      cli_args.item_functionality = malloc(1 + strlen(optarg));
      strcpy(cli_args.item_functionality, optarg);
      break;
    case QUICKSWAP:
      cli_args.quickswap = malloc(1 + strlen(optarg));
      strcpy(cli_args.quickswap, optarg);
      break;
    }
  }

  if (argc - optind < 2) {
    printf("Arguments `input_file` and `output_directory` required\n");
    exit(1);
  }

  cli_args.input_file = malloc(1 + strlen(argv[optind]));
  strcpy(cli_args.input_file, argv[optind++]);
  cli_args.output_directory = malloc(1 + strlen(argv[optind]));
  strcpy(cli_args.output_directory, argv[optind++]);

  //cli_arguments_log(cli_args);
  cli_randomize(cli_args);
}
