#include "LevelManager.h"

LevelManager::LevelManager(int screenWidth)
    : screenWidth(screenWidth),
      currentLevelIndex(0)
{
    CreateLevels();
}

void LevelManager::CreateLevels()
{
    levels.clear();

    levels.emplace_back(1, screenWidth);
    levels.emplace_back(2, screenWidth);
    levels.emplace_back(3, screenWidth);

    currentLevelIndex = 0;
}

Level& LevelManager::GetCurrentLevel()
{
    return levels[currentLevelIndex];
}

const Level& LevelManager::GetCurrentLevel() const
{
    return levels[currentLevelIndex];
}

int LevelManager::GetCurrentLevelNumber() const
{
    return currentLevelIndex + 1;
}

int LevelManager::GetTotalLevelCount() const
{
    return static_cast<int>(levels.size());
}

bool LevelManager::HasNextLevel() const
{
    return currentLevelIndex + 1 <
           static_cast<int>(levels.size());
}

bool LevelManager::MoveToNextLevel()
{
    if (!HasNextLevel())
    {
        return false;
    }

    currentLevelIndex++;
    return true;
}

void LevelManager::Reset()
{
    CreateLevels();
} 