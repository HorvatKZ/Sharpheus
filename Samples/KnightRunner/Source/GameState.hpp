#pragma once


class GameState
{
public:
	enum Difficulty {
		EASY, MEDIUM, HARD
	};

	static inline GameState& Instance() { return instance; }

	inline uint8_t GetCurrLevel() { return currLevel; }
	inline Difficulty GetDifficulty() { return currDiff; }
	std::string GetDifficultyStr();
	float GetDamageMult();
	inline bool FinishReached() {
		bool result = finishReached;
		finishReached = false;
		return result;
	}
	inline bool DoesMusicPlay() { return musicPlays; }

	inline void SetCurrLevel(uint8_t newLevel) { currLevel = newLevel; }
	inline void SetDifficulty(Difficulty newDiff) { currDiff = newDiff; }
	void SetDifficulty(const std::string& diffName);
	void ReachFinish() { finishReached = true; }
	void SetMusic(bool play) { musicPlays = play; }

private:
	static GameState instance;

	uint8_t currLevel = 0;
	Difficulty currDiff = Difficulty::MEDIUM;
	bool finishReached = false, musicPlays = false;
};

