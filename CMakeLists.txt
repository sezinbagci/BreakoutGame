cmake_minimum_required(VERSION 3.20)

project(SimulationGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# raylib
add_subdirectory(external/raylib)

# Dear ImGui
add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp
)

target_include_directories(imgui PUBLIC
    external/imgui
)

# raylib - Dear ImGui connection
add_library(rlimgui STATIC
    external/rlimgui/rlImGui.cpp
)

target_include_directories(rlimgui PUBLIC
    external/rlimgui
)

target_link_libraries(rlimgui PUBLIC
    raylib
    imgui
)

# Game source files
add_executable(SimulationGame
    src/main.cpp
    src/Game.cpp
    src/Ball.cpp
    src/Paddle.cpp
    src/Brick.cpp
)

target_include_directories(SimulationGame PRIVATE
    include
)

target_link_libraries(SimulationGame PRIVATE
    raylib
    imgui
    rlimgui
)