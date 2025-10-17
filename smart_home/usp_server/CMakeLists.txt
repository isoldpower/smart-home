add_library(usp_server STATIC
        src/UspServerConfig.cpp
        src/UspServerClient.cpp

        src/version1/UspServerRequest.cpp
        src/version1/UspServerResponse.cpp
        src/version1/UspAsyncServer.cpp

        src/exceptions/ReceivedMessageException.cpp

        src/version1/packets/SequencedPacketPoller.cpp

        src/version1/message_handlers/AcknowledgementHandler.cpp
        src/version1/message_handlers/MessageHandlerBuilder.cpp
        src/version1/message_handlers/ProtocolHandler.cpp
        src/version1/message_handlers/RequestHandler.cpp
        src/version1/message_handlers/ResponseHandler.cpp
)

if (BUILD_TESTING)
    add_executable(usp_server_tests tests/main.cpp)
    target_link_libraries(usp_server_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(usp_server_tests)
endif()

target_include_directories(usp_server INTERFACE include)
target_link_libraries(usp_server PUBLIC cxx_setup utilities daemon usp_protocol)