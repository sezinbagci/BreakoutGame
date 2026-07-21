#pragma once

#include "raylib.h"

class Ball
{
public:
    Ball();

    void AttachToPaddle(Rectangle paddle);
    void Launch(Vector2 direction);
    void Update(float deltaTime);
    void Draw() const;

    void BounceX();
    void BounceY();

    void SetPosition(Vector2 newPosition);
    void SetVelocity(Vector2 newVelocity);

    Vector2 GetPosition() const;
    Vector2 GetVelocity() const;

    float GetRadius() const;
    float GetSpeed() const;

    bool IsLaunched() const;
    bool HasFallen(float screenHeight) const;

private:
    Vector2 position;
    Vector2 velocity;

    float radius;
    float speed;

    bool launched;
};