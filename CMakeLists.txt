cmake_minimum_required(VERSION 3.19)

# Disable in-source build.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source build is not allowed. Remember to remove CMakeCache.txt and CMakeFiles/")
endif()

# Read project version
set(PROJ_VERSION_REGEX "#define IES_VERSION_.*[ \t]+(.+)")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/src/ies/version.hpp" PROJ_VERSION REGEX ${PROJ_VERSION_REGEX})
list(TRANSFORM PROJ_VERSION REPLACE ${PROJ_VERSION_REGEX} "\\1")
string(JOIN "." PROJ_VERSION ${PROJ_VERSION})

project(IES
    VERSION ${PROJ_VERSION}
    DESCRIPTION "Common C++ utility library by ITRI EOSL S-Div."
    LANGUAGES CXX
)
message(STATUS "Project [${PROJECT_NAME}] v${PROJECT_VERSION} (${CMAKE_BUILD_TYPE})")

add_subdirectory(src)
