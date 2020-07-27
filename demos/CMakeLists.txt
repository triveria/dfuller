# ref.: https://cmake.org/examples/

# Add executable called "demo_message" that is built from the source files "demo_message.cpp".
add_executable(demo_message demo_message.cpp)
add_executable(demo_Device demo_Device.cpp)

# Link the executable to the dfuller library. Since the dfuller library has
# public include directories we will use those link directories when building
# demo_message.
target_include_directories(dfuller PUBLIC ../include/)
target_link_libraries(demo_message LINK_PUBLIC dfuller)
target_link_libraries(demo_Device LINK_PUBLIC dfuller)