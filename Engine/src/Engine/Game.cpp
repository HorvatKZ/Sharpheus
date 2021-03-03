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
	}


	Game::Game() : GameBase()
	{
		Logger::Init();
		SPH_INFO("Welcome to Sharpheus!");

		ResourceManager::Init("D:\\Programming\\Sharpheus\\TestProject\\");
		EventHandler::Init(SPH_BIND(Game::WindowClosed));
		Renderer::Init();

		proj.CreateNewLevel("Level");
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
			proj.Tick(win->GetDeltaTime());
			win->StartRender();
			proj.Render();
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
