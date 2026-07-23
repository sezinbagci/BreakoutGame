#pragma once

#include "Brick.h"

#include <string>
#include <vector>

class Level
{
public:
    Level(
        int levelNumber,
        int screenWidth
    );

    int GetNumber() const;
    const std::string& GetName() const;

    std::vector<Brick>& GetBricks();
    const std::vector<Brick>& GetBricks() const;

    int GetActiveBrickCount() const;
    bool IsCompleted() const;

private:
    void BuildLevel(int screenWidth);

    void BuildGridLevel(int screenWidth);
    void BuildPyramidLevel(int screenWidth);
    void BuildDiamondLevel(int screenWidth);

    int number;
    std::string name;

    std::vector<Brick> bricks;
};