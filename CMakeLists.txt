cmake_minimum_required(VERSION 3.20)
project(game_of_life)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad include/glad/glad.h glad.c)
target_include_directories(glad PUBLIC include/)
#target_include_directories(include/)

add_executable(game_of_life main.cpp shader.cpp shader.h LifeExecutor.cpp LifeExecutor.h CustomGlfwTimer.cpp CustomGlfwTimer.h Vector2.h RenderWindow.h main.h ui/UiLayer.cpp ui/UiLayer.h main.fwd.h)

target_link_libraries(game_of_life glad)
target_link_libraries(game_of_life glfw)

if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(game_of_life PRIVATE "-Wall" "-Werror" "--warn-unused-function")
endif()
if ( MSVC )
    target_compile_options(game_of_life PRIVATE "/W4")
endif()
#target_link_libraries(game_of_life OpenGL::GL)
