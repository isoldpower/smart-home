add_library(usp_protocol STATIC
        src/messages/AcknowledgementMessage.cpp
        src/messages/RequestMessage.cpp
        src/messages/ResponseMessage.cpp
        src/messages/ProtocolMessage.cpp
        src/handlers/AcknowledgementMessageHandler.cpp
        src/handlers/RequestMessageHandler.cpp
        src/handlers/ResponseMessageHandler.cpp
        src/handlers/ProtocolMessageHandler.cpp
        include/messages/CommonMessageData.h
)

if (BUILD_TESTING)
    add_executable(usp_protocol_tests tests/main.cpp)
    target_link_libraries(usp_protocol_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(usp_protocol_tests)
endif()

target_include_directories(usp_protocol INTERFACE include)
target_link_libraries(usp_protocol PUBLIC utilities cxx_setup)