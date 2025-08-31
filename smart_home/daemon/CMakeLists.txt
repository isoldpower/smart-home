add_library(daemon STATIC
        src/SignalDaemon.cpp)

if (BUILD_TESTING)
    add_executable(daemon_tests tests/main.cpp)
    target_link_libraries(daemon_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(daemon_tests)
endif()

target_include_directories(daemon INTERFACE include)
target_link_libraries(daemon PUBLIC utilities cxx_setup)