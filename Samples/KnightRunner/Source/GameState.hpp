#pragma once


class GameState
{
public:

	static inline GameState& Instance() { return instance; }

private:
	static GameState instance;
};

