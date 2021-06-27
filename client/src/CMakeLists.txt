add_library(client common.c common.h client.c client.h sensor.c sensor.h log.h log.c)
add_executable(client_exec main.c)
target_link_libraries(client_exec client)