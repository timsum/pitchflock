cmake_minimum_required(VERSION 3.10)
project(PitchFlock)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Include directories
include_directories(include)

# Add library
file(GLOB LIB_SOURCES src/*.c)
add_library(pitchflock STATIC ${LIB_SOURCES})

# Add tests
file(GLOB TEST_SOURCES tests/*.c)
foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} pitchflock)

    # Install the test executable
    install(TARGETS ${TEST_NAME}
        RUNTIME DESTINATION bin
    )
endforeach()

# Install the library
install(TARGETS pitchflock
    ARCHIVE DESTINATION lib
)

# Install the header files
install(DIRECTORY include/
    DESTINATION include/pitchflock
)


message(STATUS "CMAKE_INSTALL_PREFIX is set to: ${CMAKE_INSTALL_PREFIX}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)