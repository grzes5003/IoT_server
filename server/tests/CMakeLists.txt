include_directories(${CMAKE_SOURCE_DIR}/src)

message(STATUS "IoT Include Directories: ${IOT_INCLUDE_DIRS}")

set(SETUP_TEST_NAME test_main)
add_executable(${SETUP_TEST_NAME} ${SETUP_TEST_NAME}.c)
target_link_libraries(${SETUP_TEST_NAME} server)
add_test(NAME MyTest COMMAND ${SETUP_TEST_NAME})


# target_link_directories()
# add_test(${SETUP_TEST_NAME} ${IOT_INCLUDE_DIRS} ${SETUP_TEST_NAME})