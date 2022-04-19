#!/bin/bash
##
## validate with cvmhvbn vxlite api
## 
rm -rf validate_vxlite_bad.txt 
rm -rf validate_vxlite_good.txt

if [ ! -f ../data/cvmhvbn/CVMHB-Ventura-Basin.dat ]; then 
  echo "need to retrieve CVMHB-Ventura-Basin.dat first!!!"
  exit 1
fi

if [ "x${UCVM_INSTALL_PATH}" != "x" ] ; then
  if [ -f $SCRIPT_DIR/../conf/ucvm_env.sh ] ; then
    SCRIPT_DIR=${UCVM_INSTALL_PATH}/bin
    source $SCRIPT_DIR/../conf/ucvm_env.sh
    ./cvmhvbn_vxlite_validate -m ../data/cvmhvbn -f ../data/cvmhvbn/CVMHB-Ventura-Basin.dat
    exit
  fi
fi

SCRIPT_DIR="$( cd "$( dirname "$0" )" && pwd )"
env DYLD_LIBRARY_PATH=${SCRIPT_DIR}/../src LD_LIBRARY_PATH=${SCRIPT_DIR}/../src ./cvmhvbn_vxlite_validate -m ../data/cvmhvbn -f ../data/cvmhvbn/CVMHB-Ventura-Basin.dat
