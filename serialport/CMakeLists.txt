cmake_minimum_required(VERSION 3.10.2)
project(serialport CXX)

set(PROJECT_PUBLIC_HEADERS
    include/${PROJECT_NAME}/namespace.hpp
    include/${PROJECT_NAME}/enumerator.hpp
    include/${PROJECT_NAME}/properties.hpp
    include/${PROJECT_NAME}/serialport.hpp
)

set(PROJECT_PUBLIC_PLATFORM_HEADERS
    include/${PROJECT_NAME}/${LIBSERIAL_PLATFORM}/properties.hpp
    include/${PROJECT_NAME}/${LIBSERIAL_PLATFORM}/serialport_impl.hpp
)

set(PROJECT_SOURCES
    src/enumerator.cpp
    src/properties.cpp
    src/serialport.cpp
    src/${LIBSERIAL_PLATFORM}/serialport_impl.cpp
)

if (SERIALPORT_ENABLE_SHARED_BUILD)
    add_library(${PROJECT_NAME} SHARED
        ${PROJECT_PUBLIC_HEADERS}
        ${PROJECT_PUBLIC_PLATFORM_HEADERS}
        ${PROJECT_SOURCES}
    )
else()
    add_library(${PROJECT_NAME} STATIC
        ${PROJECT_PUBLIC_HEADERS}
        ${PROJECT_PUBLIC_PLATFORM_HEADERS}
        ${PROJECT_SOURCES}
    )
endif()
add_library(LibSerial::SerialPort ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    PUBLIC_HEADER "${PROJECT_PUBLIC_HEADERS}"
)

target_compile_options(${PROJECT_NAME}
    PRIVATE ${LIBSERIAL_GCC_FLAGS_LIST}
)

if(LIBSERIAL_ENABLE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE ${LIBSERIAL_COVERAGE_CXX_FLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${LIBSERIAL_COVERAGE_LINKER_FLAGS})
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

install(TARGETS ${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)

install(FILES ${PROJECT_PUBLIC_PLATFORM_HEADERS}
    DESTINATION include/${PROJECT_NAME}/${LIBSERIAL_PLATFORM}
)

if(LIBSERIAL_ENABLE_TESTS)
    add_subdirectory(test)

    if(LIBSERIAL_ENABLE_COVERAGE)
        set(COVERAGE_TARGET "${PROJECT_NAME}_coverage")
        set(COVERAGE_OUTPUT_SUFFIX "coverage")
        set(COVERAGE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${COVERAGE_OUTPUT_SUFFIX}")

        message(STATUS "${PROJECT_NAME}: Build ${COVERAGE_TARGET} target to generate coverage report.")

        add_custom_target(${COVERAGE_TARGET}
            # Delete existing gcda files
            COMMAND
                find ${PROJECT_BINARY_DIR} -iname *.gcda -delete

            # Make coverage directory
            COMMAND
                ${CMAKE_COMMAND} -E make_directory ${COVERAGE_OUTPUT_PATH}

            # Run test executable
            COMMAND
                ${PROJECT_BINARY_DIR}/test/${PROJECT_NAME}_test

            # Generate coverage report
            COMMAND
                ${GCOVR_PATH}
                -r ${PROJECT_SOURCE_DIR}
                ${PROJECT_BINARY_DIR}
                -e ${PROJECT_SOURCE_DIR}/test
                --exclude-unreachable-branches
                --exclude-throw-branches
                --html-title ${PROJECT_NAME}
                --html-details
                -o ${COVERAGE_OUTPUT_SUFFIX}/${PROJECT_NAME}.html

            BYPRODUCTS ${COVERAGE_OUTPUT_PATH}/${PROJECT_NAME}.html
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            VERBATIM
            COMMENT "${PROJECT_NAME}: Coverage report generated at ${COVERAGE_OUTPUT_PATH}."
        )
    endif()
endif()
