#include <stdlib.h>
#include <time.h>
#include "./cli.h"

int main(int argc, char** argv) {
  srand(time(NULL));
  handle(argc, argv);
  return 0;
}
