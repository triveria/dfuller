
# TEST EXECUTABLE
add_executable(dfuller_tests
    dfuller.t.cpp
    tests.x.cpp
)

target_link_libraries(dfuller_tests
	dfuller
	Catch2::Catch2
)

add_test(
    NAME dfuller_tests
    COMMAND dfuller_tests
)
