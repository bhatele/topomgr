################################################################################
# Topology Manager API
################################################################################

cmake_minimum_required (VERSION 2.8)
project (topomgr)

string (REGEX MATCH "^BlueGeneP" BlueGeneP "${CMAKE_SYSTEM_NAME}")
string (REGEX MATCH "^BlueGeneQ" BlueGeneQ "${CMAKE_SYSTEM_NAME}")
string (REGEX MATCH "^XT" CrayXT "${CMAKE_SYSTEM_PROCESSOR}")

if (NOT (BlueGeneP OR BlueGeneQ OR CrayXT) )
  message(FATAL_ERROR " TopoManager module only builds on Blue Gene and XT/XE."
    "\n Perhaps you forgot to specify a toolchain file, e.g.:\n"
    "   cmake -DCMAKE_TOOLCHAIN_FILE=../BGQ-gnu.cmake ..")
endif ()

if (BlueGeneP)
  include_directories(/bgsys/drivers/ppcfloor/comm/include)
endif ()

if (BlueGeneQ)
  include_directories(/bgsys/drivers/ppcfloor/comm/include)
endif ()

# Build shared libraries by default.
get_property (DEFAULT_SHARED GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS)
option (BUILD_SHARED_LIBS "Build shared libraries?" ${DEFAULT_SHARED})

find_package(MPI REQUIRED)
add_library (topomgr CrayNid.c TopoManager.C)

add_executable (testtopo test.C)
target_link_libraries (testtopo topomgr ${MPI_LIBRARIES})

install (FILES ${PROJECT_SOURCE_DIR}/*.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install (TARGETS topomgr DESTINATION ${CMAKE_INSTALL_PREFIX}/lib EXPORT topomgr-libs)
install (EXPORT topomgr-libs DESTINATION share/cmake/topomgr)
