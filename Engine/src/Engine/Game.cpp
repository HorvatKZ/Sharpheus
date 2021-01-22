#include "pch.h"
#include "Game.hpp"

#include "Window/OpenGL_Window.hpp"


namespace Sharpheus {

	Game::Game()
	{
		win = new OpenGL_Window();
		win->SetCloseCallback(SPH_BIND(Game::Stop));
	}


	Game::~Game()
	{
		delete win;
	}


	void Game::Run()
	{
		while (isRunning) {
			win->Tick();
		}
	}


	void Game::Stop()
	{
		isRunning = false;
	}


	bool Game::IsRunning()
	{
		return isRunning;
	}

} // namespace Sharpheus
