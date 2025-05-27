cmake_minimum_required(VERSION 3.27.4)

project(zappy_gui CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

execute_process(
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/compile_commands.json ${PROJECT_SOURCE_DIR}
)

add_compile_options(-W -Wall -Wextra -std=c++20)
add_executable(${PROJECT_NAME}
  main.cpp
)

find_package(raylib REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
