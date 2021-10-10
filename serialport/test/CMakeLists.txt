cmake_minimum_required(VERSION 3.10.2)

project(serialport_test CXX)

set(TEST_PRIVATE_HEADERS
    include/${PROJECT_NAME}/test_properties.hpp
    include/${PROJECT_NAME}/test_serialport_impl.hpp
)

set(TEST_SOURCES
    src/testapp.cpp
    src/test_enumerator.cpp
    src/test_properties.cpp
    src/test_serialport.cpp
    src/test_serialport_impl.cpp
)

add_executable(${PROJECT_NAME}
    ${TEST_PRIVATE_HEADERS}
    ${TEST_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    PRIVATE_HEADER "${TEST_PRIVATE_HEADERS}"
)

target_compile_options(${PROJECT_NAME} PRIVATE ${LIBSERIAL_GCC_FLAGS_LIST})

if(LIBSERIAL_ENABLE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE ${LIBSERIAL_COVERAGE_CXX_FLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${LIBSERIAL_COVERAGE_LINKER_FLAGS})
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE gtest
    PRIVATE gtest_main
    PRIVATE LibSerial::SerialPort
)

target_include_directories(${PROJECT_NAME}
    PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

install(TARGETS ${PROJECT_NAME}
    PRIVATE_HEADER DESTINATION include/${PROJECT_NAME}
)
