
set(SRCROOT ${PROJECT_SOURCE_DIR}/Lab3/TCPClient)

# all source files
set(SRC ${SRCROOT}/tcpClient.cpp)

# define the sockets target
sfml_add_example(TCPClient
                 SOURCES ${SRC})
target_link_libraries(TCPClient PRIVATE sfml-network)
