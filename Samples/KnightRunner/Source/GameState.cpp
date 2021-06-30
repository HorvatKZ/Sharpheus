#include "pch.h"
#include "GameState.hpp"


GameState GameState::instance;

std::string GameState::GetDifficultyStr()
{
    switch (currDiff) {
        case EASY:
            return "Easy";
        case MEDIUM:
            return "Medium";
        case HARD:
            return "Hard";
    }
}


float GameState::GetDamageMult()
{
    switch (currDiff) {
        case EASY:
            return 0.7f;
        case MEDIUM:
            return 1.0f;
        case HARD:
            return 1.5f;
    }
}


void GameState::SetDifficulty(const std::string& diffName)
{
    if (diffName == "Easy") {
        currDiff = Difficulty::EASY;
    }
    else if (diffName == "Medium") {
        currDiff = Difficulty::MEDIUM;
    }
    else if (diffName == "Hard") {
        currDiff = Difficulty::HARD;
    }
}
