#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"

int main(void)
{
  const int inSize = 96;

  int *const in = malloc(sizeof(int) * inSize);
  assert(in != NULL);

  int *const out = malloc(sizeof(int) * inSize);
  assert(out != NULL);

  for(int i=0; i < inSize; ++i)
      in[i] = i;

  printf("Running DFE...");
  fflush(stdout);
  Test(inSize, in, out);
  printf("done.\n");

  for(int i=0; i < inSize; ++i)
      printf("out[%d] = %d\n", i, out[i]);

  return EXIT_SUCCESS;
}
