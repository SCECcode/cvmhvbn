#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include "unittest_defs.h"
#include "test_grid_exec.h"


int main (int argc, char *argv[])
{

  char *xmldir;

  if (argc == 2) {  
    xmldir = argv[1];
  } else {
    xmldir = NULL;
  }

  _reset_failure();

  /* Run test suites */
  suite_grid_exec(xmldir);

  if(_has_failure()) {
    return 1;
    } else {
      return 0;
  }
}
