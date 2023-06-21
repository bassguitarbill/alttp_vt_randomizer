#include <stdlib.h>
#include "./credits.h"

Credits* construct_credits() {
  Credits* credits = malloc(sizeof(Credits));
  return credits;
}
