# toolchain for Blue Gene/Q
set (CMAKE_SYSTEM_NAME BlueGeneQ-dynamic)

# Set search paths to prefer local, admin-installed wrappers for the BG backend compilers
set (BGQ_GNU_COMPILER_SEARCH_PATHS
  /usr/local/bin
  /usr/bin
  /bgsys/drivers/ppcfloor/gnu-linux/bin)

# GNU C Compilers
find_program (CMAKE_C_COMPILER       powerpc64-bgq-linux-gcc      ${BGQ_GNU_COMPILER_SEARCH_PATHS})
find_program (CMAKE_CXX_COMPILER     powerpc64-bgq-linux-g++      ${BGQ_GNU_COMPILER_SEARCH_PATHS})
find_program (CMAKE_Fortran_COMPILER powerpc64-bgq-linux-gfortran ${BGQ_GNU_COMPILER_SEARCH_PATHS})

# Make sure MPI_COMPILER wrapper matches the gnu compilers.  
# Prefer local machine wrappers to driver wrappers here too.
find_program (MPI_COMPILER NAMES mpicxx mpic++ mpiCC mpicc 
  PATHS 
  /usr/local/bin
  /usr/bin
  /bgsys/drivers/ppcfloor/comm/bin
  /bgsys/drivers/ppcfloor/comm/default/bin)
