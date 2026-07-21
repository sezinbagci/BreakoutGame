#include "Ball.h"

#include <cmath>

Ball::Ball()
    : position{0.0f, 0.0f},
      velocity{0.0f, 0.0f},
      radius(12.0f),
      speed(380.0f),
      launched(false)
{
}

void Ball::AttachToPaddle(Rectangle paddle)
{
    position = {
        paddle.x + paddle.width / 2.0f,
        paddle.y - radius - 2.0f
    };

    velocity = {0.0f, 0.0f};
    launched = false;
}

void Ball::Launch(Vector2 direction)
{
    float directionLength = std::sqrt(
        direction.x * direction.x +
        direction.y * direction.y
    );

    if (directionLength == 0.0f)
    {
        return;
    }

    direction.x /= directionLength;
    direction.y /= directionLength;

    velocity = {
        direction.x * speed,
        direction.y * speed
    };

    launched = true;
}

void Ball::Update(float deltaTime)
{
    if (!launched)
    {
        return;
    }

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Ball::Draw() const
{
    DrawCircleV(
        position,
        radius,
        RAYWHITE
    );
}

void Ball::BounceX()
{
    velocity.x *= -1.0f;
}

void Ball::BounceY()
{
    velocity.y *= -1.0f;
}

void Ball::SetPosition(Vector2 newPosition)
{
    position = newPosition;
}

void Ball::SetVelocity(Vector2 newVelocity)
{
    velocity = newVelocity;
}

Vector2 Ball::GetPosition() const
{
    return position;
}

Vector2 Ball::GetVelocity() const
{
    return velocity;
}

float Ball::GetRadius() const
{
    return radius;
}

float Ball::GetSpeed() const
{
    return speed;
}

bool Ball::IsLaunched() const
{
    return launched;
}

bool Ball::HasFallen(float screenHeight) const
{
    return position.y - radius > screenHeight;
}