if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_executable(playground playground/main.cpp
            playground/src/Entrypoint.cpp
            playground/src/commands/DelayedDaemonCommand.cpp
            playground/src/commands/ProtocolSerializationCommand.cpp)
    target_include_directories(playground INTERFACE playground/include)
else()
endif()

target_link_libraries(playground PRIVATE utilities daemon cxx_setup)
