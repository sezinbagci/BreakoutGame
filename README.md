# Breakout Game

A modular Breakout-style 2D game developed in C++ using raylib, Dear ImGui, rlImGui, and CMake.

This project was created as part of a simulation and game development internship exercise. It demonstrates game-loop management, collision detection, object-oriented programming, external library integration, and runtime debugging tools.

## Features

- Paddle movement using keyboard controls
- Mouse or touchpad-controlled launch direction
- Ball launch with the Space key
- Wall, paddle, and brick collision detection
- Brick destruction and scoring system
- Automatic game reset when the ball falls
- Pause and manual reset controls
- Runtime debug interface powered by Dear ImGui
- Adjustable paddle speed
- Modular object-oriented C++ architecture

## Technologies

- C++17
- CMake
- raylib 6.0
- Dear ImGui
- rlImGui
- MSVC / Visual Studio

## Controls

| Control | Action |
|---|---|
| `A` / Left Arrow | Move the paddle left |
| `D` / Right Arrow | Move the paddle right |
| Mouse / Touchpad | Select the ball's launch direction |
| `Space` | Launch the ball |
| `Escape` | Close the game |

## Project Structure

```text
BreakoutGame/
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
├── .gitignore
├── .gitmodules
├── CMakeLists.txt
└── README.md
```

## Architecture

The project is divided into separate classes to keep the code modular and maintainable:

- `Game`: Manages the main game loop, collisions, score, reset logic, rendering, and debug interface.
- `Ball`: Manages the ball's position, velocity, launch state, movement, and rendering.
- `Paddle`: Handles player input, paddle movement, boundaries, and rendering.
- `Brick`: Stores each brick's position, color, active state, and rendering behavior.
- `main.cpp`: Creates the `Game` object and starts the application.

## Getting Started

### Requirements

Before building the project, make sure the following tools are installed:

- Git
- CMake 3.20 or newer
- A C++17-compatible compiler
- Visual Studio with the Desktop Development with C++ workload on Windows

### Clone the Repository

The project uses Git submodules for raylib, Dear ImGui, and rlImGui. Clone it using:

```bash
git clone --recurse-submodules https://github.com/sezinbagci/BreakoutGame.git
cd BreakoutGame
```

If the repository was cloned without its submodules, run:

```bash
git submodule update --init --recursive
```

### Configure the Project

```bash
cmake -S . -B build
```

### Build the Project

```bash
cmake --build build --config Debug
```

### Run on Windows

```powershell
.\build\Debug\SimulationGame.exe
```

## Debug Panel

The Dear ImGui control panel provides real-time information and development controls:

- Current score
- Remaining brick count
- Ball state
- Ball position and velocity
- Paddle speed adjustment
- Pause control
- Manual game reset

## Game Rules

1. Move the paddle using `A/D` or the arrow keys.
2. Use the mouse or touchpad to select the ball's initial direction.
3. Press `Space` to launch the ball.
4. Break the bricks to increase the score.
5. If the ball falls below the paddle, the score and bricks reset.
6. Destroy all bricks to complete the game.

## Future Improvements

- Multiple levels
- Main menu and game-over screens
- Sound effects and background music
- Power-ups
- Different brick types
- Improved visual effects
- High-score saving

## Developer

**Sezin Bağcı**  
Computer Engineering Student