################################################################################
# Topology Manager API
################################################################################

cmake_minimum_required (VERSION 2.6)
project (topomgr)

string (REGEX MATCH "^BlueGeneP" BlueGeneP "${CMAKE_SYSTEM_NAME}")
if (NOT BlueGeneP)
  message(FATAL_ERROR " TopoManager module only builds on Blue Gene and XT/XE."
    "\n Perhaps you forgot to specify a toolchain file, e.g.:\n"
    "   cmake -DCMAKE_TOOLCHAIN_FILE=../BGP-gnu.cmake ..")
endif ()

find_package (MPI REQUIRED)

# Build shared libraries by default.
get_property (DEFAULT_SHARED GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS)
option (BUILD_SHARED_LIBS "Build shared libraries?" ${DEFAULT_SHARED})

add_library (topomgr CrayNid.c TopoManager.C)

add_executable (test test.C)
target_link_libraries (test topomgr ${MPI_LIBRARIES})

include_directories (${MPI_INCLUDE_PATH})

install (FILES ${PROJECT_SOURCE_DIR}/*.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install (TARGETS topomgr DESTINATION ${CMAKE_INSTALL_PREFIX}/lib EXPORT topomgr-libs)
install (EXPORT topomgr-libs DESTINATION share/cmake/topomgr)
