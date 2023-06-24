#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include "./md5.h"

/*
  https://stackoverflow.com/a/10324904/3295578
*/

char* md5_from_file(FILE* inFile) {
  unsigned char c[MD5_DIGEST_LENGTH];
  int i;
  MD5_CTX mdContext;
  int bytes;
  unsigned char data[1024];

  MD5_Init (&mdContext);
  fseek(inFile, 0, SEEK_SET);
  while ((bytes = fread (data, 1, 1024, inFile)) != 0)
    MD5_Update (&mdContext, data, bytes);
  MD5_Final (c,&mdContext);

  char* digest = malloc(1 + (2 * MD5_DIGEST_LENGTH));
  *digest = '\0';
  for(i = 0; i < MD5_DIGEST_LENGTH; i++) {
    char chunk[3];
    sprintf(chunk, "%02x", c[i]);
    strcat(digest, chunk);
  }
  return digest;
}
