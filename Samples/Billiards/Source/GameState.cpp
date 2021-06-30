#include "pch.h"
#include "GameState.hpp"


GameState GameState::instance;


void GameState::Reset()
{
	p1Points = 0;
	p2Points = 0;
	p1Turns = 1;
	p2Turns = 0;
	inTurn = false;
	isFinished = false;
}


void GameState::StartTurn()
{
	isP1Curr = (p1Turns > 0);
	hitHappened = false;
	hitOK = false;
	inHoleOwn = false;
	inHoleOther = false;
	inHoleWhite = false;
	inTurn = true;
}


void GameState::EndTurn()
{
	if (inHoleWhite || !hitOK || inHoleOther) {
		NextTurnPenalty();
	}
	else if (!inHoleOwn) {
		NextTurn();
	}
	inTurn = false;
}


void GameState::Hit(uint8_t ind)
{
	if (!hitHappened) {
		hitHappened = true;
		if (isP1Curr) {
			hitOK = (ind <= 8);
		} else {
			hitOK = (ind >= 8);
		}
	}
}


void GameState::InHole(uint8_t ind)
{
	if (ind == 0) {
		inHoleWhite = true;
	}
	else if (ind == 8) {
		Finish();
	}
	else if (ind < 8) {
		++p1Points;
		if (isP1Curr) {
			inHoleOwn = true;
		}
		else {
			inHoleOther = true;
		}
	}
	else {
		++p2Points;
		if (isP1Curr) {
			inHoleOther = true;
		}
		else {
			inHoleOwn = true;
		}
	}
}


void GameState::NextTurn()
{
	if (p1Turns == 1) {
		p1Turns = 0;
		p2Turns = 1;
	}
	else if (p2Turns == 1) {
		p1Turns = 1;
		p2Turns = 0;
	}
	else if (p1Turns == 2) {
		--p1Turns;
	}
	else if (p2Turns == 2) {
		--p2Turns;
	}
}


void GameState::NextTurnPenalty()
{
	if (p1Turns > 0) {
		p1Turns = 0;
		p2Turns = 2;
	}
	else {
		p1Turns = 2;
		p2Turns = 0;
	}
}


void GameState::Finish()
{
	isFinished = true;
	if (isP1Curr) {
		isP1TheWinner = (p1Points == 7);
	}
	else {
		isP1TheWinner = (p2Points != 7);
	}
}
