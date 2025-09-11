add_library(usp_protocol STATIC
        src/UspServer.cpp
        src/UspServerConfig.cpp
        src/UspServerRequest.cpp
        src/UspServerResponse.cpp
        src/handlers/CommonMessageData.cpp
        src/handlers/MessageBasisHandlerBuilder.cpp
        src/handlers/version1/acknowledgement/AcknowledgementMessage.cpp
        src/handlers/version1/acknowledgement/AcknowledgementMessageHandler.cpp
        src/handlers/version1/request/RequestMessage.cpp
        src/handlers/version1/request/RequestMessageHandler.cpp
        src/handlers/version1/response/ResponseMessage.cpp
        src/handlers/version1/response/ResponseMessageHandler.cpp
        src/handlers/version1/protocol/ProtocolMessage.cpp
        src/handlers/version1/protocol/ProtocolMessageHandler.cpp
        src/handlers/version1/MessageBasisHandler.cpp
)

if (BUILD_TESTING)
    add_executable(usp_protocol_tests tests/main.cpp)
    target_link_libraries(usp_protocol_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(usp_protocol_tests)
endif()

target_include_directories(usp_protocol INTERFACE include)
target_link_libraries(usp_protocol PUBLIC utilities web_server cxx_setup)