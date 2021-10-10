cmake_minimum_required(VERSION 3.10.2)
project(libserial CXX)

if (${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(LIBSERIAL_IS_SUBMODULE OFF)
    message(STATUS "${PROJECT_NAME}: Building standalone")
else()
    set(LIBSERIAL_IS_SUBMODULE ON)
    message(STATUS "${PROJECT_NAME}: Building as submodule")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(LIBSERIAL_PLATFORM "linux")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(LIBSERIAL_PLATFORM "windows")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
message(STATUS "${PROJECT_NAME}: Platform is ${CMAKE_SYSTEM_NAME}")

option(LIBSERIAL_ENABLE_SHARED_BUILD "Build shared library instead of static" OFF)
option(LIBSERIAL_ENABLE_COVERAGE "Enable coverage" OFF)
option(LIBSERIAL_ENABLE_TESTS "Enable tests" OFF)
option(LIBSERIAL_ENABLE_GTEST_SUBMODULE "Enable use of GoogleTest submodule" OFF)

if((NOT LIBSERIAL_IS_SUBMODULE) AND LIBSERIAL_ENABLE_TESTS AND (NOT LIBSERIAL_ENABLE_GTEST_SUBMODULE))
    message(NOTICE "${PROJECT_NAME}: Building standalone with tests enabled enables LIBSERIAL_ENABLE_GTEST_SUBMODULE option")
    message(NOTICE "${PROJECT_NAME}: Enable LIBSERIAL_ENABLE_GTEST_SUBMODULE option manually to avoid this notice")
    set(LIBSERIAL_ENABLE_GTEST_SUBMODULE ON)
endif()

set(LIBSERIAL_GCC_FLAGS "-Wall -Wextra -Wpedantic -Werror" CACHE STRING "GCC/G++ compiler flags")
separate_arguments(LIBSERIAL_GCC_FLAGS_LIST NATIVE_COMMAND "${LIBSERIAL_GCC_FLAGS}")
set(LIBSERIAL_GCC_FLAGS_LIST ${LIBSERIAL_GCC_FLAGS_LIST} CACHE INTERNAL "GCC/G++ compiler flags (list)")

if(LIBSERIAL_ENABLE_COVERAGE)
    if(NOT CMAKE_COMPILER_IS_GNUCXX)
        message(FATAL_ERROR "${PROJECT_NAME}: Coverage is only supported with GNU C++ compiler toolchain.")
    endif()

    if(NOT LIBSERIAL_ENABLE_TESTS)
        message(FATAL_ERROR "${PROJECT_NAME}: Coverage is only supported with LIBSERIAL_ENABLE_TESTS option enabled.")
    endif()

    find_program(GCOVR_PATH gcovr)
    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "${PROJECT_NAME}: gcovr not found.")
    endif()

    if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
        message(WARNING "${PROJECT_NAME}: Coverage with non-debug build may cause misleading results.")
    endif()

    set(LIBSERIAL_COVERAGE_CXX_FLAGS -g -O0 -fprofile-arcs -ftest-coverage)
    set(LIBSERIAL_COVERAGE_LINKER_FLAGS --coverage)
endif()

add_subdirectory(serialport)
add_subdirectory(external)
