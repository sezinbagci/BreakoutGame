#pragma once

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

#include <vector>

class Game
{
public:
    Game();

    void Run();

private:
    void Update();
    void Draw();
    void DrawDebugPanel();

    void CreateBricks();
    void CreateGridLevel();
    void CreatePyramidLevel();
    void CreateDiamondLevel();

    void StartNextLevel();
    void ResetGame();

    void HandleWallCollisions();
    void HandlePaddleCollision();

    void HandleBrickCollisions(
        Vector2 previousBallPosition
    );

    Vector2 CalculateAimDirection() const;
    int GetActiveBrickCount() const;

    const int screenWidth;
    const int screenHeight;

    const float playAreaLeft;
    const float playAreaRight;
    const float playAreaTop;

    Paddle paddle;
    Ball ball;

    std::vector<Brick> bricks;

    Vector2 aimDirection;

    int score;
    int currentLevel;
    int totalLevels;

    bool gamePaused;
    bool showDebugPanel;
    bool levelCompleted;
    bool gameWon;
};