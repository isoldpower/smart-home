add_library(web_server STATIC
        src/NetServer.cpp
        src/ExitCodes.cpp
        src/NetServerClientInfo.cpp
        src/NetServerConfig.cpp)

if (BUILD_TESTING)
    add_executable(web_server_tests tests/main.cpp)
    target_link_libraries(web_server_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(web_server_tests)
endif()

target_include_directories(web_server INTERFACE include)
target_link_libraries(web_server PUBLIC cxx_setup)