cmake_minimum_required(VERSION 3.8)
project(jcu_random-test CXX)

set(SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
        )

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}
        jcu-random
        gtest
        gtest_main
        )
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/include
        )

add_test(NAME ${PROJECT_NAME}-all COMMAND ${PROJECT_NAME})
