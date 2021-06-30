#include "pch.h"
#include "GameState.hpp"


GameState GameState::instance;


void GameState::Reset()
{
	points = 0;
	failed = false;
}
