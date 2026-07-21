#pragma once

#include "raylib.h"

class Paddle
{
public:
    Paddle(float screenWidth, float screenHeight);

    void Update(
        float deltaTime,
        float playAreaLeft,
        float playAreaRight
    );

    void Draw() const;
    void Reset(float screenWidth);

    Rectangle GetRectangle() const;
    float GetCenterX() const;
    float GetX() const;

    float GetSpeed() const;
    void SetSpeed(float newSpeed);

private:
    Rectangle rectangle;
    float speed;
};