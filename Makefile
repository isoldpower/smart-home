BUILD_DIR ?= build
SOURCE_DIR ?= .
CPP_COMPILER ?= g++
C_COMPILER ?= gcc
BUILD_TYPE ?= Release

rebuild:
	cmake -B "$(BUILD_DIR)" -S "$(SOURCE_DIR)" \
 		-DCMAKE_CXX_COMPILER="$(CPP_COMPILER)" \
 		-DCMAKE_C_COMPILER="$(C_COMPILER)" \
 		-DCMAKE_BUILD_TYPE="$(BUILD_TYPE)"
	cmake --build "$(BUILD_DIR)" --config "$(BUILD_TYPE)"

test:
	if ! test -d "$(BUILD_DIR)"; then "$(MAKE)" rebuild; fi
	GTEST_COLOR=1 ctest --test-dir "$(BUILD_DIR)" --build-config "$(BUILD_TYPE)" --output-on-failure
