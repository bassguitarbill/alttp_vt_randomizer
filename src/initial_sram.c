#include <stdlib.h>
#include "./initial_sram.h"

InitialSram* construct_initial_sram() {
  InitialSram* initial_sram = malloc(sizeof(InitialSram));
  return initial_sram;
}
