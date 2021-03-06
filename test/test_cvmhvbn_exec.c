/**
   test_cvmhvbn_exec.c

   uses cvmhvbn's model api,
       model_init, model_setparam, model_query, model_finalize
**/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include "test_helper.h"
#include "unittest_defs.h"
#include "test_cvmhvbn_exec.h"


int test_setup()
{
  printf("Test: model_init() and model_finalize()\n");

  char *envstr=getenv("UCVM_INSTALL_PATH");
  if(envstr != NULL) {
    if (test_assert_int(model_init(envstr, "cvmhvbn"), 0) != 0) {
      return _failure("model_init failed");
    }
  } else if (test_assert_int(model_init("..", "cvmhvbn"), 0) != 0) {
    return _failure("model_init failed");
  }

  if (test_assert_int(model_finalize(), 0) != 0) {
    return _failure("model_finalize failed");
  }

  return _success();
}

int test_setparam()
{
  printf("Test: model_setparam() with depth\n");

// Initialize the model, try to use Use UCVM_INSTALL_PATH
  char *envstr=getenv("UCVM_INSTALL_PATH");
  if(envstr != NULL) {
    if (test_assert_int(model_init(envstr, "cvmhvbn"), 0) != 0) {
      return _failure("model_init failed");
    }
  } else if (test_assert_int(model_init("..", "cvmhvbn"), 0) != 0) {
    return _failure("model_init failed");
  }

  int zmode = UCVM_COORD_GEO_DEPTH;
  if (test_assert_int(model_setparam(0, UCVM_PARAM_QUERY_MODE, zmode), 0) != 0) {
      return _failure("model_setparam failed");
  }

  // Close the model.
  if(model_finalize() != 0) {
      return _failure("model_finalize failed");
  }

  return _success();
}

/**
id,X,Y,Z,depth,lon,lat,ucvm_depth,vp63_basin,vs63_basin,vp,vs,delta
1316125,3818999.968750,281000.000000,800.000427,-0.700744,
-119.384981,34.491133,101.961148,
2116.271484,651.958191,0.000000,0.000000,-698.039279

-119.3850 34.4911 cvmh 
surface 901.049(ucvm)
vs30 385.1 
cvmhvbn/cvmh ucvm_depth(800) vp/vs/rho (2479.527/965.11/2086.785)
surface 799.299683(cvmhvbn)
utm2 -0.700317
**/
int test_query_by_depth()
{
  printf("Test: model_query() by depth\n");

  cvmhvbn_point_t pt;
  cvmhvbn_properties_t ret;

// Initialize the model, try to use Use UCVM_INSTALL_PATH
  char *envstr=getenv("UCVM_INSTALL_PATH");
  if(envstr != NULL) {
    if (test_assert_int(model_init(envstr, "cvmhvbn"), 0) != 0) {
      return _failure("model_init failed");
    }
  } else if (test_assert_int(model_init("..", "cvmhvbn"), 0) != 0) {
    return _failure("model_init failed");
  }

  int zmode = UCVM_COORD_GEO_DEPTH;
  if (test_assert_int(model_setparam(0, UCVM_PARAM_QUERY_MODE, zmode), 0) != 0) {
      return _failure("model_setparam failed");
  }

  // Query a point.
  pt.longitude = -119.3850;
  pt.latitude = 34.4911;
  pt.depth = 800;

  if (test_assert_int(model_query(&pt, &ret, 1), 0) != 0) {
      return _failure("model_query failed");
  }

  // Close the model.
  if(model_finalize() != 0) {
      return _failure("model_finalize failed");
  }

  if ( test_assert_double(ret.vs, 965.109558) ||
       test_assert_double(ret.vp, 2479.5271) ||
       test_assert_double(ret.rho, 2086.784898) ) {
     return _failure("unmatched result");
     } else {
       return _success();
  }

}

int test_query_by_elevation()
{
  printf("Test: model_query() by elevation\n");

  cvmhvbn_point_t pt;
  cvmhvbn_properties_t ret;

// Initialize the model, try to use Use UCVM_INSTALL_PATH
  char *envstr=getenv("UCVM_INSTALL_PATH");
  if(envstr != NULL) {
    if (test_assert_int(model_init(envstr, "cvmhvbn"), 0) != 0) {
      return _failure("model_init failed");
    }
  } else if (test_assert_int(model_init("..", "cvmhvbn"), 0) != 0) {
    return _failure("model_init failed");
  }

  int zmode = UCVM_COORD_GEO_ELEV;
  if (test_assert_int(model_setparam(0, UCVM_PARAM_QUERY_MODE, zmode), 0) != 0) {
      return _failure("model_setparam failed");
  }

  // Query a point.
  pt.longitude = -119.3850;
  pt.latitude = 34.4911;
  double pt_elevation = -0.700317;
  double pt_surf = 799.299683;
  pt.depth = pt_surf - pt_elevation; // elevation

  if (test_assert_int(model_query(&pt, &ret, 1), 0) != 0) {
      return _failure("model_query failed");
  }

  // Close the model.
  if(model_finalize() != 0) {
      return _failure("model_finalize failed");
  }

  if ( test_assert_double(ret.vs, 965.109558) ||
       test_assert_double(ret.vp, 2479.5271) ||
       test_assert_double(ret.rho, 2086.784898) ) {
     return _failure("unmatched result");
     } else {
       return _success();
  }
}

int test_query_points_by_elevation()
{
  printf("Test: model_query() points by elevation\n");

  FILE  *infp, *outfp;
  cvmhvbn_point_t pt;
  cvmhvbn_properties_t ret;
  double elev;
  double surf;

  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  /* Save current directory */
  getcwd(currentdir, 1000);

// ge part
  sprintf(infile, "%s/%s", currentdir, "./inputs/test_latlons_ucvm_ge.txt");
  sprintf(outfile, "%s/%s", currentdir,
          "test_latlons_ucvm_ge.out");
  sprintf(reffile, "%s/%s", currentdir,
          "./ref/test_latlons_ucvm_ge.ref");

  if (test_assert_file_exist(infile) != 0) {
    return _failure("filed not found");
  }

  // pick up every point and convert the elevation to depth in pt structure
  infp = fopen(infile, "r");
  if (infp == NULL) {
    return _failure("input file not found");
  }
  outfp = fopen(outfile, "w");
  if (outfp == NULL) {
    return _failure("output file can not be open");
  }

  char *envstr=getenv("UCVM_INSTALL_PATH");
  if(envstr != NULL) {
    if (test_assert_int(model_init(envstr, "cvmhvbn"), 0) != 0) {
      return _failure("model_init failed");
    }
  } else if (test_assert_int(model_init("..", "cvmhvbn"), 0) != 0) {
    return _failure("model_init failed");
  }

  int zmode = UCVM_COORD_GEO_ELEV;
  if (test_assert_int(model_setparam(0, UCVM_PARAM_QUERY_MODE, zmode), 0) != 0) {
      return _failure("model_setparam failed");
  }

/* process one term at a time */
  char line[1001];
  while(fgets(line, 1000, infp) != NULL) {
    if(line[0] == '#') continue; // a comment
    if (sscanf(line,"%lf %lf %lf",
         &pt.longitude,&pt.latitude,&elev) == 3) {
      surf = get_preset_cvmh_surface(pt.longitude, pt.latitude);
      pt.depth= surf - elev;
      if (test_assert_int(model_query(&pt, &ret, 1), 0) == 0) {
         fprintf(outfp,"%lf %lf %lf\n",ret.vs, ret.vp, ret.rho);
      }
    }
  }
  fclose(infp);
  fclose(outfp);

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    printf("unmatched result\n");
    printf("%s\n",outfile);
    printf("%s\n",reffile);
    return _failure("diff file");
  }

  // Close the model.
  if(model_finalize() != 0) {
      return _failure("model_finalize failed");
  }

  unlink(outfile);

  return _success();
}

int test_query_points_by_depth()
{
  printf("Test: model_query() points by depth\n");

  FILE  *infp, *outfp;
  cvmhvbn_point_t pt;
  cvmhvbn_properties_t ret;

  char infile[1280];
  char outfile[1280];
  char reffile[1280];
  char currentdir[1000];

  /* Save current directory */
  getcwd(currentdir, 1000);

  sprintf(infile, "%s/%s", currentdir, "./inputs/test-depth-ucvm.in");
  sprintf(outfile, "%s/%s", currentdir,
          "test-depth-ucvm.out");
  sprintf(reffile, "%s/%s", currentdir, "./ref/test-depth-ucvm.ref");

  if (test_assert_file_exist(infile) != 0) {
    return _failure("filed not found");
  }

  infp = fopen(infile, "r");
  if (infp == NULL) {
    return _failure("input file not found");
  }
  outfp = fopen(outfile, "w");
  if (outfp == NULL) {
    return _failure("output file can not be open");
  }

  char *envstr=getenv("UCVM_INSTALL_PATH");
  if(envstr != NULL) {
    if (test_assert_int(model_init(envstr, "cvmhvbn"), 0) != 0) {
      return _failure("model_init failed");
    }
  } else if (test_assert_int(model_init("..", "cvmhvbn"), 0) != 0) {
    return _failure("model_init failed");
  }

  int zmode = UCVM_COORD_GEO_DEPTH;
  if (test_assert_int(model_setparam(0, UCVM_PARAM_QUERY_MODE, zmode), 0) != 0) {
      return _failure("model_setparam failed");
  }

/* process one term at a time */
  char line[1001];
  while(fgets(line, 1000, infp) != NULL) {
    if(line[0] == '#') continue; // a comment
    if (sscanf(line,"%lf %lf %lf",
         &pt.longitude,&pt.latitude,&pt.depth) == 3) {
      if (test_assert_int(model_query(&pt, &ret, 1), 0) == 0) {
         fprintf(outfp,"%lf %lf %lf\n",ret.vs, ret.vp, ret.rho);
      }
    }
  }
  fclose(infp);
  fclose(outfp);

  /* Perform diff btw outfile and ref */
  if (test_assert_file(outfile, reffile) != 0) {
    printf("unmatched result\n");
    printf("%s\n",outfile);
    printf("%s\n",reffile);
    return _failure("diff file");
  }

  // Close the model.
  if(model_finalize() != 0) {
      return _failure("model_finalize failed");
  }
  unlink(outfile);

  return _success();
}

int suite_cvmhvbn_exec(const char *xmldir)
{
  suite_t suite;
  char logfile[256];
  FILE *lf = NULL;

  /* Setup test suite */
  strcpy(suite.suite_name, "suite_vcmhlabn_exec");
  suite.num_tests = 6;
  suite.tests = malloc(suite.num_tests * sizeof(test_t));
  if (suite.tests == NULL) {
    fprintf(stderr, "Failed to alloc test structure\n");
    return(1);
  }
  test_get_time(&suite.exec_time);

  /* Setup test cases */
  strcpy(suite.tests[0].test_name, "test_setup");
  suite.tests[0].test_func = &test_setup;
  suite.tests[0].elapsed_time = 0.0;

  strcpy(suite.tests[1].test_name, "test_separam");
  suite.tests[1].test_func = &test_setparam;
  suite.tests[1].elapsed_time = 0.0;

  strcpy(suite.tests[2].test_name, "test_query_by_depth");
  suite.tests[2].test_func = &test_query_by_depth;
  suite.tests[2].elapsed_time = 0.0;

  strcpy(suite.tests[3].test_name, "test_query_by_elevation");
  suite.tests[3].test_func = &test_query_by_elevation;
  suite.tests[3].elapsed_time = 0.0;

  strcpy(suite.tests[4].test_name, "test_query_points_by_elevation");
  suite.tests[4].test_func = &test_query_points_by_elevation;
  suite.tests[4].elapsed_time = 0.0;

  strcpy(suite.tests[5].test_name, "test_query_points_by_depth");
  suite.tests[5].test_func = &test_query_points_by_depth;
  suite.tests[5].elapsed_time = 0.0;

  if (test_run_suite(&suite) != 0) {
    fprintf(stderr, "Failed to execute tests\n");
    return(1);
  }

  if (xmldir != NULL) {
    sprintf(logfile, "%s/%s.xml", xmldir, suite.suite_name);
    lf = init_log(logfile);
    if (lf == NULL) {
      fprintf(stderr, "Failed to initialize logfile\n");
      return(1);
    }
    
    if (write_log(lf, &suite) != 0) {
      fprintf(stderr, "Failed to write test log\n");
      return(1);
    }

    close_log(lf);
  }

  free(suite.tests);

  return 0;
}
