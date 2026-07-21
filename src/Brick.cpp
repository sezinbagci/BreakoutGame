#include "Brick.h"

Brick::Brick(Rectangle rectangle, Color color)
    : rectangle(rectangle),
      color(color),
      active(true)
{
}

void Brick::Draw() const
{
    if (!active)
    {
        return;
    }

    DrawRectangleRounded(
        rectangle,
        0.25f,
        6,
        color
    );
}

void Brick::Destroy()
{
    active = false;
}

bool Brick::IsActive() const
{
    return active;
}

Rectangle Brick::GetRectangle() const
{
    return rectangle;
}