cmake_minimum_required (VERSION 2.8)
project(jcu-random-sample-app CXX)

add_executable(${PROJECT_NAME} sample-app.cc)
target_link_libraries(${PROJECT_NAME} jcu-random)
