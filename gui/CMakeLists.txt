cmake_minimum_required(VERSION 3.27.4)

project(zappy_gui CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

execute_process(
  COMMAND ln -sf ${PROJECT_SOURCE_DIR}/.build/compile_commands.json ${PROJECT_SOURCE_DIR}
)

add_compile_options(-W -Wall -Wextra -std=c++20)

set(SOURCES
  src/main.cpp
  src/raylib/Raylib.cpp
  src/server/ServerCommunication.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(raylib REQUIRED)
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.5.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

if (DEBUG)
  add_executable(${PROJECT_NAME}_debug ${SOURCES})

  target_include_directories(${PROJECT_NAME}_debug PUBLIC src)
  target_link_libraries(${PROJECT_NAME}_debug PRIVATE raylib raylib_cpp)
  target_compile_options(${PROJECT_NAME}_debug PRIVATE -g -O0 -DDEBUG)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib raylib_cpp)
