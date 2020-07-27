cmake_minimum_required(VERSION 3.14)

project(dfuller
    VERSION 0.1.0 # bump this
    DESCRIPTION "A device agnostic DFU library"
)

set(CMAKE_CXX_STANDARD 17)
## EXTERNAL LIBRARIES
# Catch2 unit testing library.
add_subdirectory(external/Catch2)

# Sanitizers-CMake package. Enable with '-DSANITIZE_UNDEFINED=On' at CMake command line.
set(CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/external/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH}
)
find_package(Sanitizers)

## LIBRARIES
# dfuller library
add_library(dfuller
    include/dfuller/dfuller.hpp
    src/dfuller.cpp
)

target_include_directories(dfuller
    PUBLIC include
)

target_link_libraries(dfuller usb-1.0)

## UNIT TEST SUITE
enable_testing()
add_subdirectory(tests)


set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


add_subdirectory("demos")

add_subdirectory("docs")



install(TARGETS dfuller
        DESTINATION lib
       )
       
install(DIRECTORY include/dfuller
        DESTINATION include
       )
