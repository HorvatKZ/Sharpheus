#include "pch.h"
#include "Game.hpp"

#include "Window/OpenGL_Window.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Events/EventHandler.hpp"
#include "Events/EventListener.hpp"


namespace Sharpheus {

	GameBase::GameBase()
	{
	}


	GameBase::~GameBase()
	{
		delete level;
	}


	Game::Game() : GameBase()
	{
		Logger::Init();
		SPH_INFO("Welcome to Sharpheus!");

		ResourceManager::Init();
		EventHandler::Init(SPH_BIND(Game::WindowClosed));
		Renderer::Init();

		level = new Level("Level");
		win = new OpenGL_Window();
	}


	Game::~Game()
	{
		delete win;
		Renderer::Clear();
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

}
