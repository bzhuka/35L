#include "randcpuid.h"
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the                    
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);
  void *hw;
  void *sw;
  if (rdrand_supported ())
    {
      hw = dlopen("randlibhw.so", RTLD_NOW);
      sw = NULL;
      if (hw == NULL) {
	fprintf(stderr, "Couldn't open library.\n");
	exit(1);
      }
      initialize = dlsym(hw, "rand64_init");
      if (initialize == NULL) {
      	fprintf(stderr, "Couldn't initialize.\n");
      	exit(1);
      }
      	
      rand64 = dlsym(hw, "rand64");
      if (rand64 == NULL) {
	fprintf(stderr, "Couldn't find rand64 function.\n");
	exit(1);
      }
            
      finalize = dlsym(hw, "rand64_fini");
      if (finalize == NULL) {
        fprintf(stderr, "Couldn't find rand64 function.\n");
        exit(1);
      }
    }
  else
    {
      sw = dlopen("randlibsw.so", RTLD_NOW);
      hw = NULL;
      if (sw == NULL) {
	fprintf(stderr, "Couldn't open library.\n");
	exit(1);
      }
      initialize = dlsym(sw, "rand64_init");
      if (initialize == NULL) {
        fprintf(stderr, "Couldn't initialize.\n");
        exit(1);
      }

      rand64 = dlsym(sw, "rand64");
      if (rand64 == NULL) {
	fprintf(stderr, "Couldn't find rand64 function.\n");
	exit(1);
      }
      
      finalize = dlsym(sw, "rand64_fini");
      if (finalize == NULL) {
	fprintf(stderr, "Couldn't find rand64 function.\n");
        exit(1);
      }
    }

  initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

    do
      {
	unsigned long long x = rand64 ();
	size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
	if (fwrite (&x, 1, outbytes, stdout) != outbytes)
	  {
	    output_errno = errno;
	    break;
	  }
	nbytes -= outbytes;
      }
    while (0 < nbytes);

    if (fclose (stdout) != 0)
      output_errno = errno;

    if (output_errno)
      {
	errno = output_errno;
	perror ("output");
	finalize ();
	return 1;
      }

    //finalize ();
    if (hw == NULL) {
      dlclose(sw);
    } else {
      dlclose(hw);
      }
  
    return 0;
}
