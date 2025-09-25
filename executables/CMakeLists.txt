#if (CMAKE_BUILD_TYPE MATCHES "Debug")
#    add_executable(playground playground/main.cpp
#            playground/src/Entrypoint.cpp
#            playground/src/commands/DelayedDaemonCommand.cpp
#            playground/src/commands/ProtocolSerializationCommand.cpp)
#    target_include_directories(playground INTERFACE playground/include)
#    target_link_libraries(playground PRIVATE utilities daemon cxx_setup)
#else()
#endif()

message(WARNING "Temporarily enabling playground for all build types")
add_executable(playground playground/main.cpp
        playground/src/Entrypoint.cpp
        playground/src/commands/DelayedDaemonCommand.cpp
        playground/src/commands/ProtocolSerializationCommand.cpp
        playground/src/commands/NetServerRequestCommand.cpp
        playground/src/commands/UspServerConfigCommand.cpp)
target_include_directories(playground INTERFACE playground/include)
target_link_libraries(playground PRIVATE utilities daemon usp_server cxx_setup)
