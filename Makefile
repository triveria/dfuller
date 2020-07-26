#- help             Show available targets
.PHONY : help
help: Makefile
	@echo "Makefile targets"
	@sed -n 's/^#-//p' Makefile
	@echo
	@echo Sample invocation:
	@echo "    make test test_asan"

SHELL = /bin/bash

# User-settable variables
# Processes used during CMake build
j ?= 2
BUILD_TYPE ?= Debug
BUILD_ROOT = build

# Source file enumerations
CMAKE_FILES = CMakeLists.txt tests/CMakeLists.txt
SOURCE_FILES = $(wildcard include/dfuller/*) $(wildcard src/*)
TEST_FILES = $(wildcard tests/*.hpp) $(wildcard tests/*.cpp)

# Intermediate pattern targets
$(BUILD_ROOT)/$(BUILD_TYPE)-%/Makefile : $(CMAKE_FILES)
	cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(SANITIZER_CONFIG_OPTION) -S . -B $(dir $@)
	@echo "Created:  $@"

$(BUILD_ROOT)/$(BUILD_TYPE)-%/tests/dfuller_tests : $(BUILD_ROOT)/$(BUILD_TYPE)-%/Makefile $(SOURCE_FILES) $(TEST_FILES)
	cmake --build $(dir $<) -j$(j) --target dfuller_tests
	@echo "Built:  $@"

# TESTS
#- test             Run C++ unit test suite
TEST_EXECUTABLE = $(BUILD_ROOT)/$(BUILD_TYPE)-nosan/tests/dfuller_tests

.PHONY: dfuller_tests test test_asan test_ubsan test_msan test_sanitizers

dfuller_tests: $(TEST_EXECUTABLE)

test: dfuller_tests
	$(TEST_EXECUTABLE)

ASAN_TEST_EXECUTABLE = $(BUILD_ROOT)/$(BUILD_TYPE)-asan/tests/dfuller_tests
MSAN_TEST_EXECUTABLE = $(BUILD_ROOT)/$(BUILD_TYPE)-msan/tests/dfuller_tests
UBSAN_TEST_EXECUTABLE = $(BUILD_ROOT)/$(BUILD_TYPE)-ubsan/tests/dfuller_tests

test_asan_executable: $(ASAN_TEST_EXECUTABLE)
test_asan_executable: SANITIZER_CONFIG_OPTION = -DSANITIZE_ADDRESS=On

test_msan_executable: $(MSAN_TEST_EXECUTABLE)
test_msan_executable: SANITIZER_CONFIG_OPTION = -DSANITIZE_MEMORY=On

test_ubsan_executable: $(UBSAN_TEST_EXECUTABLE)
test_ubsan_executable: SANITIZER_CONFIG_OPTION = -DSANITIZE_UNDEFINED=On

#- test_asan        Run test suite with address sanitizer
#- test_msan        Run test suite with memory sanitizer
#- test_ubsan       Run test suite with undefined behavior sanitizer
test_asan: test_asan_executable
	ASAN_OPTIONS=verbosity=1,exitcode=1 $(ASAN_TEST_EXECUTABLE)
test_msan: test_msan_executable
	MSAN_OPTIONS=verbosity=1,exitcode=1 $(MSAN_TEST_EXECUTABLE)
test_ubsan: test_ubsan_executable
	UBSAN_OPTIONS=verbosity=1,exitcode=1 $(UBSAN_TEST_EXECUTABLE)

#- test_sanitizers  Run test suite with all sanitizers (lengthy)
test_sanitizers: test test_asan test_ubsan test_msan

# Utility targets
#- all              Equivalent to 'test'
all: test

#- clean            Remove all build output
clean:
	rm -rf $(BUILD_ROOT)
