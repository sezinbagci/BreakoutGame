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
      aimDirection{0.0f, -1.0f},
      score(0),
      currentLevel(1),
      totalLevels(3),
      gamePaused(false),
      showDebugPanel(true),
      levelCompleted(false),
      gameWon(false)
{
    CreateBricks();
    ball.AttachToPaddle(paddle.GetRectangle());
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

        aimDirection = CalculateAimDirection();

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
    Vector2 position = ball.GetPosition();
    float radius = ball.GetRadius();

    if (position.x - radius <= playAreaLeft)
    {
        position.x = playAreaLeft + radius;

        ball.SetPosition(position);
        ball.BounceX();
    }

    position = ball.GetPosition();

    if (position.x + radius >= playAreaRight)
    {
        position.x = playAreaRight - radius;

        ball.SetPosition(position);
        ball.BounceX();
    }

    position = ball.GetPosition();

    if (position.y - radius <= playAreaTop)
    {
        position.y = playAreaTop + radius;

        ball.SetPosition(position);
        ball.BounceY();
    }
}

void Game::HandlePaddleCollision()
{
    Vector2 position = ball.GetPosition();
    Vector2 velocity = ball.GetVelocity();

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

        velocity.y = -std::abs(velocity.y);

        float hitPosition =
            (position.x - paddle.GetCenterX()) /
            (paddleRectangle.width / 2.0f);

        velocity.x = hitPosition * 350.0f;

        ball.SetPosition(position);
        ball.SetVelocity(velocity);
    }
}

void Game::HandleBrickCollisions(
    Vector2 previousBallPosition
)
{
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

            if (cameFromAbove || cameFromBelow)
            {
                ball.BounceY();
            }
            else
            {
                ball.BounceX();
            }

            if (GetActiveBrickCount() == 0)
            {
                paddle.Reset(
                    static_cast<float>(screenWidth)
                );

                ball.AttachToPaddle(
                    paddle.GetRectangle()
                );

                if (currentLevel == totalLevels)
                {
                    gameWon = true;
                }
                else
                {
                    levelCompleted = true;
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

    float verticalAim = -std::sqrt(
        1.0f -
        horizontalAim * horizontalAim
    );

    return {
        horizontalAim,
        verticalAim
    };
}

void Game::CreateBricks()
{
    bricks.clear();

    if (currentLevel == 1)
    {
        CreateGridLevel();
    }
    else if (currentLevel == 2)
    {
        CreatePyramidLevel();
    }
    else
    {
        CreateDiamondLevel();
    }
}

void Game::CreateGridLevel()
{
    const int rows = 5;
    const int columns = 10;

    const float brickWidth = 82.0f;
    const float brickHeight = 26.0f;
    const float gap = 8.0f;

    Color colors[rows] = {
        Color{239, 71, 111, 255},
        Color{255, 159, 67, 255},
        Color{255, 209, 102, 255},
        Color{6, 214, 160, 255},
        Color{72, 149, 239, 255}
    };

    float totalWidth =
        columns * brickWidth +
        (columns - 1) * gap;

    float startX =
        (screenWidth - totalWidth) / 2.0f;

    float startY = 130.0f;

    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            Rectangle rectangle = {
                startX +
                    column * (brickWidth + gap),

                startY +
                    row * (brickHeight + gap),

                brickWidth,
                brickHeight
            };

            bricks.emplace_back(
                rectangle,
                colors[row]
            );
        }
    }
}

void Game::CreatePyramidLevel()
{
    const int rows = 6;

    const float brickWidth = 82.0f;
    const float brickHeight = 26.0f;
    const float gap = 8.0f;

    Color colors[rows] = {
        Color{255, 89, 94, 255},
        Color{255, 146, 76, 255},
        Color{255, 202, 58, 255},
        Color{138, 201, 38, 255},
        Color{25, 130, 196, 255},
        Color{106, 76, 147, 255}
    };

    float startY = 125.0f;

    for (int row = 0; row < rows; row++)
    {
        int bricksInRow = 4 + row;

        float rowWidth =
            bricksInRow * brickWidth +
            (bricksInRow - 1) * gap;

        float startX =
            (screenWidth - rowWidth) / 2.0f;

        for (
            int column = 0;
            column < bricksInRow;
            column++
        )
        {
            Rectangle rectangle = {
                startX +
                    column * (brickWidth + gap),

                startY +
                    row * (brickHeight + gap),

                brickWidth,
                brickHeight
            };

            bricks.emplace_back(
                rectangle,
                colors[row]
            );
        }
    }
}

void Game::CreateDiamondLevel()
{
    const int rows = 7;

    const int bricksPerRow[rows] = {
        4, 6, 8, 10, 8, 6, 4
    };

    const float brickWidth = 82.0f;
    const float brickHeight = 26.0f;
    const float gap = 8.0f;

    Color colors[rows] = {
        Color{155, 93, 229, 255},
        Color{241, 91, 181, 255},
        Color{254, 228, 64, 255},
        Color{0, 187, 249, 255},
        Color{0, 245, 212, 255},
        Color{241, 91, 181, 255},
        Color{155, 93, 229, 255}
    };

    float startY = 115.0f;

    for (int row = 0; row < rows; row++)
    {
        int count = bricksPerRow[row];

        float rowWidth =
            count * brickWidth +
            (count - 1) * gap;

        float startX =
            (screenWidth - rowWidth) / 2.0f;

        for (int column = 0; column < count; column++)
        {
            Rectangle rectangle = {
                startX +
                    column * (brickWidth + gap),

                startY +
                    row * (brickHeight + gap),

                brickWidth,
                brickHeight
            };

            bricks.emplace_back(
                rectangle,
                colors[row]
            );
        }
    }
}

void Game::StartNextLevel()
{
    currentLevel++;
    levelCompleted = false;
    gamePaused = false;

    paddle.Reset(
        static_cast<float>(screenWidth)
    );

    CreateBricks();

    ball.AttachToPaddle(
        paddle.GetRectangle()
    );

    aimDirection = CalculateAimDirection();
}

void Game::ResetGame()
{
    score = 0;
    currentLevel = 1;

    gamePaused = false;
    levelCompleted = false;
    gameWon = false;

    paddle.Reset(
        static_cast<float>(screenWidth)
    );

    CreateBricks();

    ball.AttachToPaddle(
        paddle.GetRectangle()
    );

    aimDirection = CalculateAimDirection();
}

int Game::GetActiveBrickCount() const
{
    int activeBrickCount = 0;

    for (const Brick& brick : bricks)
    {
        if (brick.IsActive())
        {
            activeBrickCount++;
        }
    }

    return activeBrickCount;
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(Color{18, 22, 35, 255});

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
            currentLevel,
            totalLevels
        ),
        screenWidth / 2 - 75,
        35,
        24,
        RAYWHITE
    );

    DrawText(
        TextFormat("SKOR: %d", score),
        screenWidth - 190,
        35,
        25,
        RAYWHITE
    );

    DrawRectangleLines(
        static_cast<int>(playAreaLeft),
        static_cast<int>(playAreaTop),
        static_cast<int>(
            playAreaRight - playAreaLeft
        ),
        screenHeight - 130,
        Color{70, 80, 110, 255}
    );

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
            TextFormat("TOPLAM SKOR: %d", score),
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
        currentLevel,
        totalLevels
    );

    ImGui::Text("Score: %d", score);

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

    if (ImGui::Button("Oyunu Sifirla"))
    {
        ResetGame();
    }

    ImGui::End();
}