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
  src/parser/Parser.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)
    FetchContent_MakeAvailable(raylib)
endif()
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

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib raylib_cpp)

# Target for the debug build
add_executable(${PROJECT_NAME}_debug EXCLUDE_FROM_ALL ${SOURCES})
target_include_directories(${PROJECT_NAME}_debug PUBLIC src)
target_link_libraries(${PROJECT_NAME}_debug PRIVATE raylib raylib_cpp)
target_compile_options(${PROJECT_NAME}_debug PRIVATE -g -O0 -DDEBUG)
add_custom_target(debug DEPENDS ${PROJECT_NAME}_debug)
