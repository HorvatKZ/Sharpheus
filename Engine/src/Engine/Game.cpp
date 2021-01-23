#include "pch.h"
#include "Game.hpp"

#include "Window/OpenGL_Window.hpp"
#include "ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	Game::Game()
	{
		ResourceManager::Init();
		Logger::Init();

		SPH_LOG_INFO("Loggers initialized!");

		win = new OpenGL_Window();
		win->SetCloseCallback(SPH_BIND(Game::Stop));
	}


	Game::~Game()
	{
		delete win;
		ResourceManager::Clear();

		SPH_LOG_INFO("Game successfully exited");
		Logger::Clear();
	}


	void Game::Run()
	{
		Image* image = ResourceManager::GetImage("D:/Programming/Sharpheus/Assets/Branding/sharpheus_promo.png");
		while (isRunning) {
			win->PollEvents();
			// Do ticks
			win->StartRender();
			image->Render({ -0.5, -0.5 }, { 0.5, 0.5 });
			win->EndRender();
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
