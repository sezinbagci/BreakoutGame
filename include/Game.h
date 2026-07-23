#pragma once

#include "Ball.h"
#include "LevelManager.h"
#include "Paddle.h"

class Game
{
public:
    Game();

    void Run();

private:
    void Update();
    void Draw();
    void DrawDebugPanel();

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

    LevelManager levelManager;

    Vector2 aimDirection;

    int score;

    bool gamePaused;
    bool showDebugPanel;
    bool levelCompleted;
    bool gameWon;
};