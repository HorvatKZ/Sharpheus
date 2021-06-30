#pragma once


class GameState
{
public:
	static inline GameState& Instance() { return instance; }

	void Reset();

	inline uint32_t GetPoints() { return points; }
	inline void AddPoint() { ++points; }
	inline bool IsFailed() { return failed; }
	inline void Fail() { failed = true; }

private:
	static GameState instance;

	uint32_t points = 0;
	bool failed = false;
};

