#include "pch.h"
#include "Game.hpp"

#include "Window/OpenGL_Window.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Events/EventHandler.hpp"
#include "Events/EventListener.hpp"


namespace Sharpheus {

	Game::Game()
	{
		Logger::Init();
		SPH_INFO("Welcome to Sharpheus!");

		ResourceManager::Init();
		EventHandler::Init(SPH_BIND(Game::WindowClosed));

		win = new OpenGL_Window();
		level = new Level();
	}


	Game::~Game()
	{
		delete win;
		ResourceManager::Clear();
		EventHandler::Clear();

		SPH_INFO("Game successfully exited");
		Logger::Clear();
	}


	void Game::Run()
	{
		while (isRunning) {
			win->PollEvents();
			level->Tick(win->GetDeltaTime());
			win->StartRender();
			level->Render();
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


	void Game::WindowClosed(const WindowClosedEvent& e)
	{
		SPH_LOG("Window closed");
		Stop();
	}

} // namespace Sharpheus
