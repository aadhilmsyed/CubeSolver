cmake_minimum_required(VERSION 3.10)
project(RubiksCube)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add this line to help find Qt6
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@6")

# Add OpenGL components
find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)

add_executable(RubiksCube
    src/main.cpp
    src/cube.cpp
    src/mainwindow.cpp
    src/cuberenderer.h
    src/cuberenderer.cpp
)

target_link_libraries(RubiksCube PRIVATE 
    Qt6::Widgets
    Qt6::OpenGLWidgets
)