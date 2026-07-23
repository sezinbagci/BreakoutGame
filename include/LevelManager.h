#pragma once

#include "Level.h"

#include <vector>

class LevelManager
{
public:
    explicit LevelManager(int screenWidth);

    Level& GetCurrentLevel();
    const Level& GetCurrentLevel() const;

    int GetCurrentLevelNumber() const;
    int GetTotalLevelCount() const;

    bool HasNextLevel() const;
    bool MoveToNextLevel();

    void Reset();

private:
    void CreateLevels();

    int screenWidth;
    int currentLevelIndex;

    std::vector<Level> levels;
};