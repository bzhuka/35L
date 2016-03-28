#include "randlib.h"
#include <stdio.h>
#include <stdlib.h>

static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
__attribute__((constructor))
void
rand64_init (void)
{
  urandstream = fopen ("/dev/urandom", "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
extern unsigned long long
rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Finalize the software rand64 implementation.  */
__attribute__((destructor))
void
rand64_fini (void)
{
  fclose (urandstream);
}
