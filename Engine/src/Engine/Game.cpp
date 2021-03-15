#include "pch.h"
#include "Game.hpp"

#include "Window/OpenGL_Window.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Events/EventHandler.hpp"
#include "Events/EventListener.hpp"

#ifdef SPH_EXPORTED
	#include "ExportedProjectData.hpp"
	#include "FileUnits/OSPaths.hpp"
#endif


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

		win = new OpenGL_Window();
		ResourceManager::Init("D:\\Programming\\Sharpheus\\TestProject\\");
		EventHandler::Init(SPH_BIND(Game::WindowClosed));

#ifdef SPH_EXPORTED
		proj = new Project(projectData, OSPaths::Get(OSPaths::Folder::EXEC_FOLDER));
		win->SetProps(proj->GetWinProps());
#endif 
	}


	Game::~Game()
	{
		delete win;
		delete proj;
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
			proj->Tick(win->GetDeltaTime());
			win->StartRender();
			proj->Render();
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
