

cmake_minimum_required(VERSION 2.6)

project(Schemer)

set(SCHEMER_VERSION_MAJOR 0)
set(SCHEMER_VERSION_MINOR 1)
set(SCHEMER_VERSION_PATCH 1)
set(SCHEMER_VERSION "${SCHEMER_VERSION_MAJOR}.${SCHEMER_VERSION_MINOR}.${SCHEMER_VERSION_PATCH}")

message(STATUS "Configuring ${PROJECT_NAME} ${SCHEMER_VERSION}")


## Configuration options ###############################

###
### Configuration settings
###
# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include CACHE PATH  "Installation directory for header files"
)
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR share/${CMAKE_PROJECT_NAME}/cmake)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for
 CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

set(SCHEMER_BUILD_TESTS FALSE CACHE BOOL "Build schemer tests.")

configure_file (
  "${PROJECT_SOURCE_DIR}/include/schemer/SchemerConfig.h.in"
  "${PROJECT_BINARY_DIR}/include/schemer/SchemerConfig.h"
)

## End configuration options ###########################

## FIND PACKAGES ########################################

## Boost ##
# Disable auto-linking
add_definitions(-DBOOST_ALL_NO_LIB)
find_package(Boost 1.36.0 REQUIRED)

# Yaml
find_package(YamlCpp REQUIRED)

## END FIND PACKAGES ####################################

# Include directories with header files
set(SCHEMER_INCLUDE_DIRS
  ${Boost_INCLUDE_DIRS}
  ${YAML_CPP_INCLUDE_DIRS}
  ${PROJECT_BINARY_DIR}/include  # So that we find SchemerConfig.h
  ${PROJECT_SOURCE_DIR}/include
)

include_directories(
  ${SCHEMER_INCLUDE_DIRS}
)

set_property(TARGET PROPERTY PROJECT_LABEL "Schemer")


set(schemer_Header_Files
  include/schemer/Assert.h
  include/schemer/Enumeration.h
  include/schemer/List.h
  include/schemer/Map.h
  include/schemer/Parse.h
  include/schemer/Registry.h
  include/schemer/Scalar.h
  include/schemer/Schemer.h
  include/schemer/Types.h
  include/schemer/Variant.h
  include/schemer/YamlTranscode.h
)
source_group("Header Files" FILES ${schemer_Header_Files})

set(schemer_Header_Files__detail
  include/schemer/detail/Enumeration.h
  include/schemer/detail/List.h
  include/schemer/detail/Map.h
  include/schemer/detail/Type.h
  include/schemer/detail/Variant.h
)
source_group("Header Files" FILES ${schemer_Header_Files})

set(schemer_Source_Files
  src/Parse.cpp
)
source_group("Source Files" FILES ${schemer_Source_Files})

set(schemer_Files
  ${schemer_Header_Files}
  ${schemer_Source_Files}
)

#####################
## Schemer Library ##
#####################
add_library(schemer SHARED
  ${schemer_Files}
)

# Libraries we need to link to
target_link_libraries(schemer
  ${Boost_LIBRARIES}
  ${YAML_CPP_LIBRARIES}
)

## Installation
install(TARGETS schemer
  EXPORT SchemerTargets
  PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR} COMPONENT dev
  LIBRARY DESTINATION ${INSTALL_LIB_DIR} COMPONENT shlib
  ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
)
install(DIRECTORY include/ DESTINATION ${INSTALL_INCLUDE_DIR}
  COMPONENT dev
  PATTERN "*.in" EXCLUDE
)

###
### Configuration file settings
###
# Add all targets to the build-tree export set
export(TARGETS schemer FILE "${PROJECT_BINARY_DIR}/SchemerTargets.cmake")
 
# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE Schemer)
 
# Create the *Config.cmake and *ConfigVersion.cmake files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")

# ... for the build tree
set(CONF_INCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/include"  "${PROJECT_BINARY_DIR}/include"
)
configure_file(cmake/SchemerConfig.cmake.in
  "${PROJECT_BINARY_DIR}/SchemerConfig.cmake" @ONLY)

# ... for the install tree
set(CONF_INCLUDE_DIRS "\${SCHEMER_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(cmake/SchemerConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SchemerConfig.cmake" @ONLY)

# ... for both
configure_file(cmake/SchemerConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/SchemerConfigVersion.cmake" @ONLY)

# Install the *Config.cmake and *ConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SchemerConfig.cmake"
  "${PROJECT_BINARY_DIR}/SchemerConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# Install the export set for use with the install-tree
install(EXPORT SchemerTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)


##########
# Tests ##
##########
if(SCHEMER_BUILD_TESTS)
  add_subdirectory(tests)
endif(SCHEMER_BUILD_TESTS)



