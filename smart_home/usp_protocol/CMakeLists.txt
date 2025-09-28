add_library(usp_protocol STATIC
        src/CommonMessageData.cpp
        src/MessageHandlerBuilder.cpp
        src/MessageUnion.cpp
        src/version1/acknowledgement/AcknowledgementMessage.cpp
        src/version1/acknowledgement/AcknowledgementMessageHandler.cpp
        src/version1/request/RequestMessage.cpp
        src/version1/request/RequestMessageHandler.cpp
        src/version1/response/ResponseMessage.cpp
        src/version1/response/ResponseMessageHandler.cpp
        src/version1/protocol/ProtocolMessage.cpp
        src/version1/protocol/ProtocolMessageHandler.cpp
        src/version1/MessageBasisHandler.cpp
        src/version1/ProtocolVersion1.cpp
        src/version1/BinaryMessageHandler.cpp)

if (BUILD_TESTING)
    add_executable(usp_protocol_tests tests/main.cpp)
    target_link_libraries(usp_protocol_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(usp_protocol_tests)
endif()

target_include_directories(usp_protocol INTERFACE include)
target_link_libraries(usp_protocol PUBLIC utilities web_server cxx_setup)