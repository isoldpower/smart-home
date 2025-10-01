add_library(usp_server STATIC
        src/UspServerConfig.cpp
        src/UspServerRequest.cpp
        src/UspServerResponse.cpp
        src/UspSyncServer.cpp
        src/packets/SequencedPacketPoller.cpp
        src/model/UspServer.cpp)

if (BUILD_TESTING)
    add_executable(usp_server_tests tests/main.cpp)
    target_link_libraries(usp_server_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(usp_server_tests)
endif()

target_include_directories(usp_server INTERFACE include)
target_link_libraries(usp_server PUBLIC cxx_setup utilities usp_protocol)