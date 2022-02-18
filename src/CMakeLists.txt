set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C5105: macro expansion producing 'defined' has undefined behavior
#   caused by /Zc:preprocessor will expose windows header inconform to standard.
# C4251: 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'
#   workaround for windows dll problem currently.
set(COMMON_MSVC_FLAGS "/permissive-;/W4;/WX;/Zi;/Zc:__cplusplus;/Zc:preprocessor;/wd5105;/DFMT_HEADER_ONLY")
set(COMMON_GCC_FLAGS "-pedantic;-Wall;-Wextra;-Werror;-g;-fPIC;-DFMT_HEADER_ONLY")

if (MSVC)
    set(COMMON_FLAGS ${COMMON_MSVC_FLAGS})
else()
    set(COMMON_FLAGS ${COMMON_GCC_FLAGS})
endif()

if (UNIX)
    # UNIX use following env vars set by script outside.
    # gtest: GTEST_ROOT
endif()
if (WIN32)
    if (MSVC)
        unset(ENV{GTEST_ROOT})
    endif()
    if (MINGW)
        set(ENV{GTEST_ROOT} $ENV{INSTALL_ROOT}/gtest/1.10.0/mingw64-10.2.0)
    endif()
endif()

find_package(GTest REQUIRED)

# Header only libraries
file(TO_CMAKE_PATH $ENV{FMT_ROOT}/include FMT_INCLUDE_DIRS)
if(EXISTS ${FMT_INCLUDE_DIRS}/fmt/format.h)
   message(STATUS "Found FMT: " ${FMT_INCLUDE_DIRS})
else()
   message(FATAL_ERROR "Not found FMT in " ${FMT_INCLUDE_DIRS})
endif()

file(TO_CMAKE_PATH $ENV{JSON_ROOT}/include JSON_INCLUDE_DIRS)
if(EXISTS ${JSON_INCLUDE_DIRS}/nlohmann/json.hpp)
   message(STATUS "Found Nlohmann Json: " ${JSON_INCLUDE_DIRS})
else()
   message(FATAL_ERROR "Not found Nlohmann Json in " ${JSON_INCLUDE_DIRS})
endif()

set_property(GLOBAL PROPERTY PROP_HEADERS)
set_property(GLOBAL PROPERTY PROP_SOURCES)
set_property(GLOBAL PROPERTY PROP_PUBLIC_HEADERS)
set_property(GLOBAL PROPERTY PROP_TEST_SOURCES)

add_subdirectory(ies)

get_property(HEADERS GLOBAL PROPERTY PROP_HEADERS)
get_property(SOURCES GLOBAL PROPERTY PROP_SOURCES)
get_property(PUBLIC_HEADERS GLOBAL PROPERTY PROP_PUBLIC_HEADERS)
get_property(TEST_SOURCES GLOBAL PROPERTY PROP_TEST_SOURCES)

add_library(ies SHARED ${PUBLIC_HEADERS} ${HEADERS} ${SOURCES})
if (NOT MSVC)
    set_target_properties(ies PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
endif()
set_target_properties(ies PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_include_directories(ies SYSTEM PUBLIC ${FMT_INCLUDE_DIRS} ${JSON_INCLUDE_DIRS})
target_include_directories(ies PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_BINARY_DIR}/exports>"
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
set_target_properties(ies PROPERTIES CXX_STANDARD 17)
if (MSVC)
    target_compile_options(ies PRIVATE ${COMMON_MSVC_FLAGS}
        /Dies_EXPORTS
        $<$<CONFIG:DEBUG>:/Od>
        $<$<CONFIG:RELEASE>:/O2>
    )
else()
    target_compile_options(ies PRIVATE ${COMMON_GCC_FLAGS}
        -Dies_EXPORTS
        $<$<CONFIG:DEBUG>:-O0 -time>
        $<$<CONFIG:RELEASE>:-O2>
    )
endif()

find_package(benchmark QUIET)
if (benchmark_FOUND)
    add_executable(benchmark app/benchmark.cpp)
    set_target_properties(benchmark PROPERTIES CXX_STANDARD 17)
    target_compile_options(benchmark PUBLIC ${COMMON_FLAGS})
    target_link_libraries(benchmark ies benchmark::benchmark)
endif()

add_executable(performance app/performance.cpp)
set_target_properties(performance PROPERTIES CXX_STANDARD 17)
target_compile_options(performance PUBLIC ${COMMON_FLAGS})
target_link_libraries(performance ies)

add_executable(test-all app/test-all.cpp ${TEST_SOURCES})
target_include_directories(test-all PUBLIC ${FMT_INCLUDE_DIRS})
set_target_properties(test-all PROPERTIES CXX_STANDARD 17)
target_compile_options(test-all PUBLIC ${COMMON_FLAGS})
target_link_libraries(test-all GTest::gtest ies)

set(CPP11ONLY_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/Common/AdjacentArrayRangeTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/Common/AdjacentVectorRangeTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/Common/EndianTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/Common/IntegralRangeTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/Common/IntegralRangeListTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/Common/SmartEnumTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/StdUtil/EraseIfTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/StdUtil/FormatStringTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/StdUtil/IsInTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/StdUtil/ReverseEqualRangeTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ies/String/SplitStringTest.cpp
)

if (NOT MSVC)
    # Note: MSVC doesn't have /std:c++11, so only test in gcc/mingw.
    add_executable(test-cpp11only app/test-cpp11only.cpp ${CPP11ONLY_TEST_SOURCES})
    target_include_directories(test-cpp11only PUBLIC ${FMT_INCLUDE_DIRS})
    target_compile_options(test-cpp11only PUBLIC -std=c++11 ${COMMON_GCC_FLAGS})
    target_link_libraries(test-cpp11only GTest::gtest ies)
endif()

set(MINGW_LIBRARY_NAMES gcc_s_seh-1 stdc++-6 winpthread-1)
set(MINGW_LIBRARIES)
foreach (LIB ${MINGW_LIBRARY_NAMES})
    set(MINGW_LIB_PATH $ENV{MINGW64_ROOT}/bin/lib${LIB}.dll)
    if (EXISTS ${MINGW_LIB_PATH} AND NOT IS_DIRECTORY ${MINGW_LIB_PATH})
        file(TO_CMAKE_PATH ${MINGW_LIB_PATH} FOUND_MINGW_LIB)
        list(APPEND MINGW_LIBRARIES ${FOUND_MINGW_LIB})
    endif()
endforeach()

if (MINGW)
    add_custom_command(TARGET test-all POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${MINGW_LIBRARIES} ${GTEST_LIBRARY}
        $<TARGET_FILE_DIR:test-all>
    )
endif()

# Install
include(GenerateExportHeader)
generate_export_header(ies EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/ies/ies_export.h)
install(FILES ${CMAKE_BINARY_DIR}/exports/ies/ies_export.h DESTINATION include/ies)

message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
install(
    TARGETS ies
    EXPORT ies-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/ies)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ies-config.cmake.in
    ies-config.cmake
    INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR
)

write_basic_package_version_file(
    ies-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    EXPORT ies-targets
    FILE ies-targets.cmake
    NAMESPACE ies::
    DESTINATION ${CONFIG_INSTALL_DIR}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ies-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ies-config-version.cmake
    DESTINATION ${CONFIG_INSTALL_DIR}
)

foreach(file ${PUBLIC_HEADERS})
    string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR} include header ${file})
    get_filename_component(dir ${header} DIRECTORY)
    install(FILES ${file} DESTINATION ${dir})
endforeach()

add_test(test test-all)
