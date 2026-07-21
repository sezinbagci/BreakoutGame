#include "Paddle.h"

Paddle::Paddle(float screenWidth, float screenHeight)
    : rectangle{
          screenWidth / 2.0f - 75.0f,
          screenHeight - 60.0f,
          150.0f,
          20.0f
      },
      speed(500.0f)
{
}

void Paddle::Update(
    float deltaTime,
    float playAreaLeft,
    float playAreaRight
)
{
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        rectangle.x += speed * deltaTime;
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        rectangle.x -= speed * deltaTime;
    }

    if (rectangle.x < playAreaLeft)
    {
        rectangle.x = playAreaLeft;
    }

    if (rectangle.x + rectangle.width > playAreaRight)
    {
        rectangle.x =
            playAreaRight - rectangle.width;
    }
}

void Paddle::Draw() const
{
    DrawRectangleRounded(
        rectangle,
        0.5f,
        10,
        Color{72, 149, 239, 255}
    );
}

void Paddle::Reset(float screenWidth)
{
    rectangle.x =
        screenWidth / 2.0f -
        rectangle.width / 2.0f;
}

Rectangle Paddle::GetRectangle() const
{
    return rectangle;
}

float Paddle::GetCenterX() const
{
    return rectangle.x + rectangle.width / 2.0f;
}

float Paddle::GetX() const
{
    return rectangle.x;
}

float Paddle::GetSpeed() const
{
    return speed;
}

void Paddle::SetSpeed(float newSpeed)
{
    speed = newSpeed;
}