#include "Game.h"

#include "imgui.h"
#include "rlImGui.h"

#include <algorithm>
#include <cmath>

Game::Game()
    : screenWidth(1000),
      screenHeight(700),
      playAreaLeft(10.0f),
      playAreaRight(990.0f),
      playAreaTop(90.0f),
      paddle(1000.0f, 700.0f),
      levelManager(1000),
      aimDirection{0.0f, -1.0f},
      score(0),
      gamePaused(false),
      showDebugPanel(true),
      levelCompleted(false),
      gameWon(false)
{
    ball.AttachToPaddle(
        paddle.GetRectangle()
    );
}

void Game::Run()
{
    InitWindow(
        screenWidth,
        screenHeight,
        "Breakout Game"
    );

    SetTargetFPS(60);
    rlImGuiSetup(true);

    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }

    rlImGuiShutdown();
    CloseWindow();
}

void Game::Update()
{
    if (gameWon)
    {
        if (IsKeyPressed(KEY_R))
        {
            ResetGame();
        }

        return;
    }

    if (levelCompleted)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            StartNextLevel();
        }

        return;
    }

    if (gamePaused)
    {
        return;
    }

    float deltaTime = GetFrameTime();

    paddle.Update(
        deltaTime,
        playAreaLeft,
        playAreaRight
    );

    if (!ball.IsLaunched())
    {
        ball.AttachToPaddle(
            paddle.GetRectangle()
        );

        aimDirection =
            CalculateAimDirection();

        if (IsKeyPressed(KEY_SPACE))
        {
            ball.Launch(aimDirection);
        }

        return;
    }

    Vector2 previousBallPosition =
        ball.GetPosition();

    ball.Update(deltaTime);

    HandleWallCollisions();
    HandlePaddleCollision();

    HandleBrickCollisions(
        previousBallPosition
    );

    if (
        ball.HasFallen(
            static_cast<float>(screenHeight)
        )
    )
    {
        ResetGame();
    }
}

void Game::HandleWallCollisions()
{
    Vector2 position =
        ball.GetPosition();

    float radius =
        ball.GetRadius();

    if (
        position.x - radius <=
        playAreaLeft
    )
    {
        position.x =
            playAreaLeft + radius;

        ball.SetPosition(position);
        ball.BounceX();
    }

    position = ball.GetPosition();

    if (
        position.x + radius >=
        playAreaRight
    )
    {
        position.x =
            playAreaRight - radius;

        ball.SetPosition(position);
        ball.BounceX();
    }

    position = ball.GetPosition();

    if (
        position.y - radius <=
        playAreaTop
    )
    {
        position.y =
            playAreaTop + radius;

        ball.SetPosition(position);
        ball.BounceY();
    }
}

void Game::HandlePaddleCollision()
{
    Vector2 position =
        ball.GetPosition();

    Vector2 velocity =
        ball.GetVelocity();

    Rectangle paddleRectangle =
        paddle.GetRectangle();

    if (
        velocity.y > 0.0f &&
        CheckCollisionCircleRec(
            position,
            ball.GetRadius(),
            paddleRectangle
        )
    )
    {
        position.y =
            paddleRectangle.y -
            ball.GetRadius();

        velocity.y =
            -std::abs(velocity.y);

        float hitPosition =
            (position.x -
             paddle.GetCenterX()) /
            (paddleRectangle.width / 2.0f);

        velocity.x =
            hitPosition * 350.0f;

        ball.SetPosition(position);
        ball.SetVelocity(velocity);
    }
}

void Game::HandleBrickCollisions(
    Vector2 previousBallPosition
)
{
    std::vector<Brick>& bricks =
        levelManager
            .GetCurrentLevel()
            .GetBricks();

    for (Brick& brick : bricks)
    {
        if (!brick.IsActive())
        {
            continue;
        }

        Rectangle brickRectangle =
            brick.GetRectangle();

        if (
            CheckCollisionCircleRec(
                ball.GetPosition(),
                ball.GetRadius(),
                brickRectangle
            )
        )
        {
            brick.Destroy();
            score += 10;

            bool cameFromAbove =
                previousBallPosition.y +
                ball.GetRadius() <=
                brickRectangle.y;

            bool cameFromBelow =
                previousBallPosition.y -
                ball.GetRadius() >=
                brickRectangle.y +
                brickRectangle.height;

            if (
                cameFromAbove ||
                cameFromBelow
            )
            {
                ball.BounceY();
            }
            else
            {
                ball.BounceX();
            }

            if (
                levelManager
                    .GetCurrentLevel()
                    .IsCompleted()
            )
            {
                paddle.Reset(
                    static_cast<float>(
                        screenWidth
                    )
                );

                ball.AttachToPaddle(
                    paddle.GetRectangle()
                );

                if (
                    levelManager.HasNextLevel()
                )
                {
                    levelCompleted = true;
                }
                else
                {
                    gameWon = true;
                }
            }

            break;
        }
    }
}

Vector2 Game::CalculateAimDirection() const
{
    Vector2 ballPosition =
        ball.GetPosition();

    float horizontalAim =
        (GetMouseX() - ballPosition.x) /
        300.0f;

    horizontalAim = std::clamp(
        horizontalAim,
        -0.90f,
        0.90f
    );

    float verticalAim =
        -std::sqrt(
            1.0f -
            horizontalAim * horizontalAim
        );

    return {
        horizontalAim,
        verticalAim
    };
}

void Game::StartNextLevel()
{
    if (!levelManager.MoveToNextLevel())
    {
        gameWon = true;
        levelCompleted = false;
        return;
    }

    levelCompleted = false;
    gamePaused = false;

    paddle.Reset(
        static_cast<float>(screenWidth)
    );

    ball.AttachToPaddle(
        paddle.GetRectangle()
    );

    aimDirection =
        CalculateAimDirection();
}

void Game::ResetGame()
{
    score = 0;

    gamePaused = false;
    levelCompleted = false;
    gameWon = false;

    levelManager.Reset();

    paddle.Reset(
        static_cast<float>(screenWidth)
    );

    ball.AttachToPaddle(
        paddle.GetRectangle()
    );

    aimDirection =
        CalculateAimDirection();
}

int Game::GetActiveBrickCount() const
{
    return levelManager
        .GetCurrentLevel()
        .GetActiveBrickCount();
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(
        Color{18, 22, 35, 255}
    );

    DrawText(
        "BREAKOUT",
        30,
        25,
        38,
        SKYBLUE
    );

    DrawText(
        TextFormat(
            "BOLUM: %d/%d",
            levelManager
                .GetCurrentLevelNumber(),

            levelManager
                .GetTotalLevelCount()
        ),
        screenWidth / 2 - 75,
        35,
        24,
        RAYWHITE
    );

    DrawText(
        TextFormat(
            "SKOR: %d",
            score
        ),
        screenWidth - 190,
        35,
        25,
        RAYWHITE
    );

    DrawRectangleLines(
        static_cast<int>(playAreaLeft),
        static_cast<int>(playAreaTop),
        static_cast<int>(
            playAreaRight -
            playAreaLeft
        ),
        screenHeight - 130,
        Color{70, 80, 110, 255}
    );

    const std::vector<Brick>& bricks =
        levelManager
            .GetCurrentLevel()
            .GetBricks();

    for (const Brick& brick : bricks)
    {
        brick.Draw();
    }

    paddle.Draw();
    ball.Draw();

    if (
        !ball.IsLaunched() &&
        !levelCompleted &&
        !gameWon
    )
    {
        Vector2 ballPosition =
            ball.GetPosition();

        Vector2 aimLineEnd = {
            ballPosition.x +
                aimDirection.x * 110.0f,

            ballPosition.y +
                aimDirection.y * 110.0f
        };

        DrawLineEx(
            ballPosition,
            aimLineEnd,
            4.0f,
            YELLOW
        );

        DrawCircleV(
            aimLineEnd,
            6.0f,
            YELLOW
        );

        DrawText(
            "Mouse ile yon sec - SPACE ile firlat",
            screenWidth / 2 - 195,
            screenHeight - 105,
            20,
            YELLOW
        );
    }

    DrawText(
        "Hareket: Sol/Sag ok veya A/D",
        30,
        screenHeight - 30,
        18,
        LIGHTGRAY
    );

    if (gamePaused)
    {
        DrawText(
            "OYUN DURAKLATILDI",
            screenWidth / 2 - 155,
            screenHeight / 2,
            28,
            YELLOW
        );
    }

    if (levelCompleted)
    {
        DrawRectangle(
            0,
            0,
            screenWidth,
            screenHeight,
            Color{0, 0, 0, 170}
        );

        DrawText(
            "BOLUM TAMAMLANDI!",
            screenWidth / 2 - 175,
            screenHeight / 2 - 35,
            32,
            GREEN
        );

        DrawText(
            "Sonraki bolum icin SPACE tusuna bas",
            screenWidth / 2 - 225,
            screenHeight / 2 + 20,
            20,
            RAYWHITE
        );
    }

    if (gameWon)
    {
        DrawRectangle(
            0,
            0,
            screenWidth,
            screenHeight,
            Color{0, 0, 0, 190}
        );

        DrawText(
            "TEBRIKLER! OYUNU KAZANDIN!",
            screenWidth / 2 - 245,
            screenHeight / 2 - 45,
            30,
            GOLD
        );

        DrawText(
            TextFormat(
                "TOPLAM SKOR: %d",
                score
            ),
            screenWidth / 2 - 110,
            screenHeight / 2 + 5,
            22,
            RAYWHITE
        );

        DrawText(
            "Yeniden baslamak icin R tusuna bas",
            screenWidth / 2 - 210,
            screenHeight / 2 + 45,
            20,
            RAYWHITE
        );
    }

    rlImGuiBegin();
    DrawDebugPanel();
    rlImGuiEnd();

    EndDrawing();
}

void Game::DrawDebugPanel()
{
    if (!showDebugPanel)
    {
        return;
    }

    ImGui::Begin(
        "Game Control Panel",
        &showDebugPanel
    );

    ImGui::Text(
        "Level: %d / %d",
        levelManager
            .GetCurrentLevelNumber(),

        levelManager
            .GetTotalLevelCount()
    );

    ImGui::Text(
        "Level Name: %s",
        levelManager
            .GetCurrentLevel()
            .GetName()
            .c_str()
    );

    ImGui::Text(
        "Score: %d",
        score
    );

    ImGui::Text(
        "Kalan blok: %d",
        GetActiveBrickCount()
    );

    ImGui::Text(
        "Top durumu: %s",
        ball.IsLaunched()
            ? "Hareket ediyor"
            : "Bekliyor"
    );

    ImGui::Separator();

    float paddleSpeed =
        paddle.GetSpeed();

    if (
        ImGui::SliderFloat(
            "Paddle Speed",
            &paddleSpeed,
            100.0f,
            1000.0f
        )
    )
    {
        paddle.SetSpeed(paddleSpeed);
    }

    Vector2 ballPosition =
        ball.GetPosition();

    Vector2 ballVelocity =
        ball.GetVelocity();

    ImGui::Text(
        "Ball Position: %.1f, %.1f",
        ballPosition.x,
        ballPosition.y
    );

    ImGui::Text(
        "Ball Velocity: %.1f, %.1f",
        ballVelocity.x,
        ballVelocity.y
    );

    ImGui::Checkbox(
        "Oyunu Duraklat",
        &gamePaused
    );

    if (
        ImGui::Button(
            "Oyunu Sifirla"
        )
    )
    {
        ResetGame();
    }

    ImGui::End();
}