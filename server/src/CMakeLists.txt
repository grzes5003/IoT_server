add_library(server server.h server.c control.c control.h common.c common.h tpool.h tpool.c log.c log.h)
add_executable(demo main.c)
target_link_libraries(demo server)