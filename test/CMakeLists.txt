cmake_minimum_required(VERSION 3.13)

find_package(GTest CONFIG REQUIRED)

FILE(GLOB_RECURSE GLDEMO_TEST_SOURCE
        ${PROJECT_SOURCE_DIR}/test/**/*.cpp
        ${PROJECT_SOURCE_DIR}/test/**/*.h
        ${PROJECT_SOURCE_DIR}/test/*.cpp
        ${PROJECT_SOURCE_DIR}/test/*.h
        )
message(STATUS "find test sources: ${GLDEMO_TEST_SOURCE}")
add_executable(gldemo_test ${GLDEMO_TEST_SOURCE} ${GLDEMO_SOURCE})
target_link_libraries(gldemo_test Qt6::Core Qt6::Widgets GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(gldemo_test)
