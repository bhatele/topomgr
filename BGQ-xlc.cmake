# toolchain for Blue Gene/Q
set (CMAKE_SYSTEM_NAME BlueGeneQ-dynamic)

# Set search paths to prefer local, admin-installed wrappers for the BG backend compilers
set (BGQ_XLC_COMPILER_SEARCH_PATHS
  /usr/local/bin
  /usr/bin
  /bgsys/drivers/ppcfloor/gnu-linux/powerpc64-bgq-linux/bin
  /soft/compilers/ibmcmp-may2015/vacpp/bg/12.1/bin)

# XLC C Compilers
find_program (CMAKE_C_COMPILER       powerpc64-bgq-linux-xlc ${BGQ_XLC_COMPILER_SEARCH_PATHS})
find_program (CMAKE_CXX_COMPILER     powerpc64-bgq-linux-xlC ${BGQ_XLC_COMPILER_SEARCH_PATHS})
find_program (CMAKE_Fortran_COMPILER powerpc64-bgq-linux-xlf ${BGQ_XLC_COMPILER_SEARCH_PATHS})

# Make sure MPI_COMPILER wrapper matches the gnu compilers.  
# Prefer local machine wrappers to driver wrappers here too.
find_program (MPI_COMPILER NAMES mpixlc mpixlcxx
  PATHS 
  /usr/local/bin
  /usr/bin
  /bgsys/drivers/ppcfloor/comm/bin
  /bgsys/drivers/ppcfloor/comm/default/bin)
