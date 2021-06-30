#pragma once


class GameState
{
public:
	static inline GameState& Instance() { return instance; }

	void Reset();

	inline uint32_t GetP1Points() { return p1Points; }
	inline uint32_t GetP2Points() { return p2Points; }
	inline uint32_t GetP1Turns() { return p1Turns; }
	inline uint32_t GetP2Turns() { return p2Turns; }
	inline bool IsInTurn() { return inTurn; }
	inline bool IsFinished() { return isFinished; }
	inline bool IsP1TheWinner() { return isP1TheWinner; }

	void StartTurn();
	void EndTurn();

	void Hit(uint8_t ind);
	void InHole(uint8_t ind);

private:
	static GameState instance;

	uint32_t p1Points, p2Points, p1Turns, p2Turns;
	bool inTurn, isFinished, isP1TheWinner;
	bool isP1Curr, hitHappened, hitOK, inHoleOwn, inHoleOther, inHoleWhite;

	void NextTurn();
	void NextTurnPenalty();
	void Finish();
};

