add_library(usp_server STATIC
        src/UspServer.cpp
        src/UspServerConfig.cpp
        src/UspServerRequest.cpp
        src/UspServerResponse.cpp
        src/packets/SequencedPacketPoller.cpp)

if (BUILD_TESTING)
    add_executable(usp_server_tests tests/main.cpp)
    target_link_libraries(usp_server_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(usp_server_tests)
endif()

target_include_directories(usp_server INTERFACE include)
target_link_libraries(usp_server PUBLIC cxx_setup utilities usp_protocol)