﻿cmake_minimum_required(VERSION 3.15)
project("tbgj4")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory("libs/raylib")

include_directories("fonts")

add_executable(${PROJECT_NAME} "src/main.cpp" "src/pallette.h")

target_link_libraries(${PROJECT_NAME} "raylib")

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()