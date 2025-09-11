add_library(usp_protocol STATIC
        src/messages/AcknowledgementMessage.cpp
        src/messages/RequestMessage.cpp
        src/messages/ResponseMessage.cpp
        src/messages/ProtocolMessage.cpp
        src/handlers/AcknowledgementMessageHandler.cpp
        src/handlers/RequestMessageHandler.cpp
        src/handlers/ResponseMessageHandler.cpp
        src/handlers/ProtocolMessageHandler.cpp
        src/net/NetServer.cpp
        src/UspServer.cpp
        src/UspServerConfig.cpp
        src/UspServerRequest.cpp
        include/UspServerRequest.h
        src/UspServerResponse.cpp
        include/UspServerResponse.h
        include/handlers/MessageHandlerBuilder.h
        include/handlers/model/ProtocolBasisHandler.h
        include/handlers/version1/MessageBasisHandler.h
)

if (BUILD_TESTING)
    add_executable(usp_protocol_tests tests/main.cpp)
    target_link_libraries(usp_protocol_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(usp_protocol_tests)
endif()

target_include_directories(usp_protocol INTERFACE include)
target_link_libraries(usp_protocol PUBLIC utilities cxx_setup)