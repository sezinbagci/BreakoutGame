#include "Level.h"

Level::Level(
    int levelNumber,
    int screenWidth
)
    : number(levelNumber)
{
    BuildLevel(screenWidth);
}

void Level::BuildLevel(int screenWidth)
{
    bricks.clear();

    switch (number)
    {
        case 1:
            name = "Grid";
            BuildGridLevel(screenWidth);
            break;

        case 2:
            name = "Pyramid";
            BuildPyramidLevel(screenWidth);
            break;

        case 3:
            name = "Diamond";
            BuildDiamondLevel(screenWidth);
            break;

        default:
            name = "Unknown";
            break;
    }
}

void Level::BuildGridLevel(int screenWidth)
{
    const int rows = 5;
    const int columns = 10;

    const float brickWidth = 82.0f;
    const float brickHeight = 26.0f;
    const float gap = 8.0f;

    const Color colors[rows] = {
        Color{239, 71, 111, 255},
        Color{255, 159, 67, 255},
        Color{255, 209, 102, 255},
        Color{6, 214, 160, 255},
        Color{72, 149, 239, 255}
    };

    const float totalWidth =
        columns * brickWidth +
        (columns - 1) * gap;

    const float startX =
        (screenWidth - totalWidth) / 2.0f;

    const float startY = 130.0f;

    for (int row = 0; row < rows; row++)
    {
        for (
            int column = 0;
            column < columns;
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

void Level::BuildPyramidLevel(int screenWidth)
{
    const int rows = 6;

    const float brickWidth = 82.0f;
    const float brickHeight = 26.0f;
    const float gap = 8.0f;

    const Color colors[rows] = {
        Color{255, 89, 94, 255},
        Color{255, 146, 76, 255},
        Color{255, 202, 58, 255},
        Color{138, 201, 38, 255},
        Color{25, 130, 196, 255},
        Color{106, 76, 147, 255}
    };

    const float startY = 125.0f;

    for (int row = 0; row < rows; row++)
    {
        const int bricksInRow = 4 + row;

        const float rowWidth =
            bricksInRow * brickWidth +
            (bricksInRow - 1) * gap;

        const float startX =
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

void Level::BuildDiamondLevel(int screenWidth)
{
    const int rows = 7;

    const int bricksPerRow[rows] = {
        4, 6, 8, 10, 8, 6, 4
    };

    const float brickWidth = 82.0f;
    const float brickHeight = 26.0f;
    const float gap = 8.0f;

    const Color colors[rows] = {
        Color{155, 93, 229, 255},
        Color{241, 91, 181, 255},
        Color{254, 228, 64, 255},
        Color{0, 187, 249, 255},
        Color{0, 245, 212, 255},
        Color{241, 91, 181, 255},
        Color{155, 93, 229, 255}
    };

    const float startY = 115.0f;

    for (int row = 0; row < rows; row++)
    {
        const int brickCount =
            bricksPerRow[row];

        const float rowWidth =
            brickCount * brickWidth +
            (brickCount - 1) * gap;

        const float startX =
            (screenWidth - rowWidth) / 2.0f;

        for (
            int column = 0;
            column < brickCount;
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

int Level::GetNumber() const
{
    return number;
}

const std::string& Level::GetName() const
{
    return name;
}

std::vector<Brick>& Level::GetBricks()
{
    return bricks;
}

const std::vector<Brick>& Level::GetBricks() const
{
    return bricks;
}

int Level::GetActiveBrickCount() const
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

bool Level::IsCompleted() const
{
    return GetActiveBrickCount() == 0;
}