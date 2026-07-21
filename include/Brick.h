#pragma once

#include "raylib.h"

class Brick
{
public:
    Brick(Rectangle rectangle, Color color);

    void Draw() const;
    void Destroy();

    bool IsActive() const;
    Rectangle GetRectangle() const;

private:
    Rectangle rectangle;
    Color color;
    bool active;
};