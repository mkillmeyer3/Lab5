
set(SRCROOT ${PROJECT_SOURCE_DIR}/Lab3/TCPServer)

# all source files
set(SRC ${SRCROOT}/tcpServer.cpp)


# define the sockets target
sfml_add_example(TCPServer
                 SOURCES ${SRC})
target_link_libraries(TCPServer PRIVATE sfml-network)
