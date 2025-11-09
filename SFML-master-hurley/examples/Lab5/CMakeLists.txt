set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/Lab5)

# all source files
set(server ${SRCROOT}/server.cpp)
set(client ${SRCROOT}/client.cpp)

# define the Lab5 target
sfml_add_example(server
                 SOURCES ${server}
                 DEPENDS sfml-network)

sfml_add_example(client
                 SOURCES ${client}
                 DEPENDS sfml-network)