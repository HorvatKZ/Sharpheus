#include "pch.h"
#include "Game.hpp"

#include "Window/OpenGL_Window.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Events/EventHandler.hpp"
#include "Events/EventListener.hpp"


namespace Sharpheus {

	GameBase::GameBase()
	{
		win = new OpenGL_Window();
	}


	GameBase::~GameBase()
	{
		delete win;
		delete level;
	}


	void GameBase::Run()
	{
		while (isRunning) {
			win->PollEvents();
			level->Tick(win->GetDeltaTime());
			win->StartRender();
			level->Render();
			win->EndRender();
		}
	}


	void GameBase::Stop()
	{
		isRunning = false;
	}


	bool GameBase::IsRunning()
	{
		return isRunning;
	}


	Game::Game() : GameBase()
	{
		Logger::Init();
		SPH_INFO("Welcome to Sharpheus!");

		ResourceManager::Init();
		EventHandler::Init(SPH_BIND(Game::WindowClosed));

		level = new Level("Level");
	}


	Game::~Game()
	{
		ResourceManager::Clear();
		EventHandler::Clear();

		SPH_INFO("Game successfully exited");
		Logger::Clear();
	}


	void Game::WindowClosed(const WindowClosedEvent& e)
	{
		SPH_LOG("Window closed");
		Stop();
	}

} // namespace Sharpheus
