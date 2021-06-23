include_directories(${CMAKE_SOURCE_DIR})

message(STATUS "IoT Include Directories: ${IOT_INCLUDE_DIRS}")

set(SETUP_TEST_NAME test_main)
add_executable(${SETUP_TEST_NAME} ${SETUP_TEST_NAME}.c)
add_test(${SETUP_TEST_NAME} ${IOT_INCLUDE_DIRS} ${SETUP_TEST_NAME})