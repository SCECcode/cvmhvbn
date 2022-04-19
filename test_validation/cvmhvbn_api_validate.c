/*
 * @file cvmhvbn_api_validate.c
 * @brief test with a full set of validation points in depth
 * @author - SCEC
 * @version 1.0
 *
 * Tests the CVMHVBN library by loading it and use ucvm api as a
 * UCVM pulgin model
 *
 *
 *  ./cvmhvbn_api_validate -f validate_vxlite_good.txt
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>
#include <ucvm_model_dtypes.h>
#include "cvmhvbn.h"

int validate_debug = 0;


/*********************************************/
typedef struct dat_entry_t 
{
  int x_idx;
  int y_idx;
  int z_idx;
  int vp_idx;
  int vs_idx;
  int rho_idx;
  int depth_idx;
} dat_entry_t;

typedef struct dat_data_t 
{
  double x;
  double y;
  double z;
  double vp;
  double vs;
  double rho;
  double depth;
} dat_data_t;

dat_entry_t dat_entry;

/*
X,Y,Z,depth,vp63_basin,vs63_basin
383000.000000,3744000.000000,-15000.000000,0.000000,-99999.000000,-99999.000000
384000.000000,3744000.000000,-15000.000000,0.000000,-99999.000000,-99999.000000
*/
FILE *_process_datfile(char *fname) {

  char dat_line[1028];
  FILE *fp = fopen(fname, "r");
  if (fp == NULL) {
    fprintf(stderr,"CVMHVBN_VALIDATE_API: FAIL: Unable to open the validation data file %s\n", fname);
    exit(1);
  }
  /* read the title line */
  if (fgets(dat_line, 1028, fp) == NULL) {
    fprintf(stderr,"CVMHVBN_VALIDATE_API: FAIL: Unable to extract validation data file %s\n", fname);
    fclose(fp);
    exit(1);
  }

  /* Strip terminating newline */
  int slen = strlen(dat_line);
  if ((slen > 0) && (dat_line[slen-1] == '\n')) {
    dat_line[slen-1] = '\0';
  }

  char delimiter[] = ",";
  char *p = strtok(dat_line, delimiter);
  int counter=0;

// X,Y,Z,depth,vp63_basin,vs63_basin
  while(p != NULL)
  {
    if(validate_debug) { printf("CVMHVBN_VALIDATE_API:'%s'\n", p); }
    if(strcmp(p,"X")==0)
      dat_entry.x_idx=counter;
    else if(strcmp(p,"Y")==0)
      dat_entry.y_idx=counter;
    else if(strcmp(p,"Z")==0)
      dat_entry.z_idx=counter;
    else if(strcmp(p,"vp63_basin")==0)
      dat_entry.vp_idx=counter;
    else if(strcmp(p,"vs63_basin")==0)
      dat_entry.vs_idx=counter;
    else if(strcmp(p,"depth")==0)
      dat_entry.depth_idx=counter;
    p = strtok(NULL, delimiter);
    counter++;
  }
  return fp;
}

int _next_datfile(FILE *fp, dat_data_t *dat) {

  char dat_line[1028];
  if (fgets(dat_line, 1028, fp) == NULL) {
    return(1); 
  }

  char delimiter[] = ",";
  char *p = strtok(dat_line, delimiter);
  int counter=0;

//X,Y,Z,depth,vp63_basin,vs63_basin
//383000.000000,3744000.000000,-15000.000000,0.000000,-99999.000000,-99999.000000
  while(p != NULL) {
    double val = atof(p);
    if(counter == dat_entry.x_idx)
        dat->x=val;
      else if (counter == dat_entry.y_idx)
        dat->y=val;
      else if (counter == dat_entry.z_idx)
        dat->z=val;
      else if (counter == dat_entry.vs_idx)
        dat->vs=val;
      else if (counter == dat_entry.vp_idx)
        dat->vp=val;
      else if (counter == dat_entry.depth_idx)
        dat->depth=val;
    p = strtok(NULL, delimiter);
    counter++;
  }
  return(0);
}


/*********************************************/

int _compare_double(double f1, double f2) {
  double precision = 0.00001;
  if (((f1 - precision) < f2) && ((f1 + precision) > f2)) {
    return 0; // good
    } else {
      return 1; // bad
  }
}

/* Usage function */
void usage() {
  printf("     vx_cvmhvbn_valiate - (c) SCEC\n");
  printf("Extract velocities from a simple GOCAD voxet. Accepts\n");
  printf("geographic coordinates and UTM Zone 11, NAD27 coordinates in\n");
  printf("X Y Z columns. Z is expressed as elevation offset by default.\n\n");
  printf("\tusage: vx_cvmhvbn_validate [-d] [-z dep/elev/off] -f file.dat\n\n");
  printf("Flags:\n");
  printf("\t-f point.dat\n\n");
  printf("\t-d enable debug/verbose mode\n\n");
  printf("\t-z directs use of dep/elev/off for Z column (default is dep).\n\n");
  printf("Output format is:\n");
  printf("\tvp vs rho\n\n");
  exit (0);
}

extern char *optarg;
extern int optind, opterr, optopt;

/**
 * Initializes CVMHLABN in standalone mode as ucvm plugin 
 * api.
 *
 * @param argc The number of arguments.
 * @param argv The argument strings.
 * @return A zero value indicating success.
 */
int main(int argc, char* const argv[]) {

	// Declare the structures.
	cvmhvbn_point_t pt;
	cvmhvbn_properties_t ret;
        int zmode=UCVM_COORD_GEO_DEPTH;
        int rc;
        int opt;
        char datfile[100]="";
        dat_data_t dat;
        int tcount=0;  // total entry
        int mcount=0;  // real mismatch
        int mmcount=0; // fake mismatch -- no data
        int okcount=0;
        FILE *ofp= fopen("validate_api_bad.txt","w");
        FILE *oofp= fopen("validate_api_good.txt","w");
        fprintf(ofp,"X,Y,Z,depth,vp63_basin,vs63_basin\n");
        fprintf(oofp,"X,Y,Z,depth,vp63_basin,vs63_basin\n");

        /* Parse options */
        while ((opt = getopt(argc, argv, "df:z:h")) != -1) {
          switch (opt) {
          case 'f':
            strcpy(datfile, optarg);
            break;
          case 'z':
            if (strcasecmp(optarg, "dep") == 0) {
              zmode = UCVM_COORD_GEO_DEPTH;
            } else if (strcasecmp(optarg, "elev") == 0) {
              zmode = UCVM_COORD_GEO_ELEV;
            } else {
              fprintf(stderr, "CVMHVBN_VALIDATE_API: Invalid coord type %s", optarg);
              usage();
              exit(0);
            }
            break;
          case 'd':
            validate_debug=1;
            break;
          case 'h':
            usage();
            exit(0);
            break;
          default: /* '?' */
            usage();
            exit(1);
          }
        }

      
        FILE *fp=_process_datfile(datfile);

	// Initialize the model. 
        // try to use Use UCVM_INSTALL_PATH
        char *envstr=getenv("UCVM_INSTALL_PATH");
        if(envstr != NULL) {
	   assert(cvmhvbn_init(envstr, "cvmhvbn") == 0);
           } else {
	     assert(cvmhvbn_init("..", "cvmhvbn") == 0);
        }
	printf("CVMHVBN_VALIDATE_API: Loaded the model successfully.\n");

        assert(cvmhvbn_setparam(0, UCVM_PARAM_QUERY_MODE, zmode) == 0);
	printf("CVMHVBN_VALIDATE_API: Set model zmode successfully.\n");

        rc=_next_datfile(fp, &dat);
        while(rc==0) {
              tcount++;

              pt.longitude = dat.x;
              pt.latitude = dat.y;

              pt.depth = dat.depth;

	      rc=cvmhvbn_query(&pt, &ret, 1); // rc 0 is okay

              if(validate_debug) {fprintf(stderr, "CVMHVBN_VALIDATE_API:   with.. %lf %lf %lf\n",pt.longitude, pt.latitude, pt.depth); }
              if(rc == 0) {

                if(validate_debug) {
                   fprintf(stderr,"CVMHVBN_VALIDATE_API:     vs:%lf vp:%lf rho:%lf\n\n",ret.vs, ret.vp, ret.rho);
                }

                // is result matching ?
                if(_compare_double(ret.vs, dat.vs) ||
                             _compare_double(ret.vp, dat.vp)) { 

/*** special case.. only in lr

CVMHVBN_VALIDATE_API:356000.000000,3754000.000000,-100.000114
CVMHVBN_VALIDATE_API: dat.vs(-99999.000000),dat.vp(1480.000000)
CVMHVBN_VALIDATE_API:   ret vs:(-1.000000) ret vp:(-1.000000)

**/
                     // okay if ( dat.vp == -99999, dat.vs== -99999 ) and (ret.vs == -1, ret.vp == -1) 
                     if (!_compare_double(ret.vs, -1.0) && !_compare_double(ret.vp, -1.0) &&
                              !_compare_double(dat.vs, -99999.0) && !_compare_double(dat.vp, -99999.0)) {
                       mmcount++;  // just -1 vs -99999
                       fprintf(oofp,"%lf,%lf,%lf,%lf,%lf,%lf\n",dat.x,dat.y,dat.z,dat.depth,dat.vp,dat.vs);
                       } else {
                         fprintf(stderr,"\nCVMHVBN_VALIDATE_API:Mismatching -\n");
                         fprintf(stderr,"CVMHVBN_VALIDATE_API:%lf,%lf,%lf\n",dat.x, dat.y, dat.z);
                         fprintf(stderr,"CVMHVBN_VALIDATE_API: dat.vs(%lf),dat.vp(%lf)\n",dat.vs, dat.vp);
                         fprintf(stderr,"CVMHVBN_VALIDATE_API:   ret vs:(%lf) ret vp:(%lf)\n",ret.vs, ret.vp);
                         mcount++;  // real mismatch
                         fprintf(ofp,"%lf,%lf,%lf,%lf,%lf,%lf\n",dat.x,dat.y,dat.z,dat.depth,dat.vp,dat.vs);
                      }
                    } else {
                         okcount++;
                         fprintf(oofp,"%lf,%lf,%lf,%lf,%lf,%lf\n",dat.x,dat.y,dat.z,dat.depth,dat.vp,dat.vs);
                  }
                } else { // rc=1 
                   if(validate_debug) printf("CVMHVBN_VALIDATE_API: BAD,  %lf %lf %lf\n",pt.longitude, pt.latitude, pt.depth);
              }
          rc=_next_datfile(fp, &dat);
        }

        fprintf(stderr,"CVMHVBN_VALIDATE_API: %d mismatch out of %d \n", mcount, tcount);
        fprintf(stderr,"CVMHVBN_VALIDATE_API: good with matching values(%d) mmcount(%d) \n",okcount, mmcount );
	assert(cvmhvbn_finalize() == 0);
	printf("CVMHVBN_VALIDATE_API:Model closed successfully.\n");


        fclose(fp);
        fclose(ofp);
        fclose(oofp);

	return 0;
}
