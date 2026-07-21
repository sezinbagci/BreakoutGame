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
      gamePaused(false),
      showDebugPanel(true)
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
    float deltaTime = GetFrameTime();

    if (gamePaused)
    {
        return;
    }

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

    if (ball.HasFallen(
        static_cast<float>(screenHeight)
    ))
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

    const int brickRows = 5;
    const int brickColumns = 10;

    const float brickWidth = 82.0f;
    const float brickHeight = 26.0f;
    const float brickGap = 8.0f;

    Color rowColors[brickRows] = {
        Color{239, 71, 111, 255},
        Color{255, 159, 67, 255},
        Color{255, 209, 102, 255},
        Color{6, 214, 160, 255},
        Color{72, 149, 239, 255}
    };

    float totalWidth =
        brickColumns * brickWidth +
        (brickColumns - 1) * brickGap;

    float startX =
        (screenWidth - totalWidth) / 2.0f;

    float startY = 130.0f;

    for (int row = 0; row < brickRows; row++)
    {
        for (
            int column = 0;
            column < brickColumns;
            column++
        )
        {
            Rectangle rectangle = {
                startX +
                    column *
                    (brickWidth + brickGap),

                startY +
                    row *
                    (brickHeight + brickGap),

                brickWidth,
                brickHeight
            };

            bricks.emplace_back(
                rectangle,
                rowColors[row]
            );
        }
    }
}

void Game::ResetGame()
{
    score = 0;
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
    int activeBrickCount =
        GetActiveBrickCount();

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

    if (!ball.IsLaunched())
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

    if (activeBrickCount == 0)
    {
        DrawText(
            "TEBRIKLER! BUTUN BLOKLARI KIRDIN!",
            screenWidth / 2 - 250,
            screenHeight / 2,
            26,
            GREEN
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