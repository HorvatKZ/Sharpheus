#include "pch.h"
#include "Game.hpp"

#include "Engine.hpp"
#include "ProjectControl.hpp"
#include "Window/OpenGL_Window.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Events/EventHandler.hpp"
#include "Events/EventListener.hpp"

#ifdef SPH_EXPORTED
	#include "ExportedProjectData.hpp"
	#include "FileUtils/OSPaths.hpp"
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
#ifdef SPH_EXPORTED
		Engine::Init(BehaviorCreator::Instance());

		EventHandler::Init(SPH_BIND(Game::WindowClosed));

		win = new OpenGL_Window();

		proj = new Project(projectData, OSPaths::Get(OSPaths::Folder::EXEC_FOLDER), projectData.name + "proj.sharpheus");
		win->SetProps(proj->GetWinProps());
#endif 
	}


	Game::~Game()
	{
		delete win;
		delete proj;
		EventHandler::Clear();
		Engine::Clear();
	}


	void Game::Run()
	{
		while (isRunning) {
			win->PollEvents();
			proj->Tick(win->GetTime());
			win->StartRender();
			proj->Render();
			win->EndRender();
			
			if (ProjectControl::NeedToExit()) {
				Stop();
			}
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
