# Breakout Game

A Breakout-style 2D game developed in C++ using raylib, Dear ImGui, rlImGui and CMake.

The project was created as part of a simulation and game development internship exercise. It demonstrates game-loop management, collision detection, modular C++ architecture and graphical debugging controls.

## Features

- Paddle movement with arrow keys or A/D
- Mouse-controlled launch direction
- Space key to launch the ball
- Ball collision with walls, paddle and bricks
- Brick destruction and score system
- Complete game reset when the ball falls
- Runtime debug panel with Dear ImGui
- Adjustable paddle speed
- Pause and reset controls
- Modular object-oriented C++ structure

## Technologies

- C++17
- CMake
- raylib 6.0
- Dear ImGui
- rlImGui
- MSVC / Visual Studio 2026

## Controls

| Control | Action |
|---|---|
| A / Left Arrow | Move paddle left |
| D / Right Arrow | Move paddle right |
| Mouse / Touchpad | Select launch direction |
| Space | Launch the ball |
| Escape | Close the game |

## Project Structure

```text
SimulationGame/
├── include/
│   ├── Ball.h
│   ├── Brick.h
│   ├── Game.h
│   └── Paddle.h
├── src/
│   ├── Ball.cpp
│   ├── Brick.cpp
│   ├── Game.cpp
│   ├── Paddle.cpp
│   └── main.cpp
├── external/
│   ├── imgui/
│   ├── raylib/
│   └── rlimgui/
├── CMakeLists.txt
└── README.md