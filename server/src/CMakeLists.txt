add_library(server server.h server.c control.c control.h common.c common.h tpool.h tpool.c log.c log.h)
add_executable(server_exec main.c)
target_link_libraries(server_exec server)