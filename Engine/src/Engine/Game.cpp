#include "pch.h"
#include "Game.hpp"

#include "Engine.hpp"
#include "ProjectControl.hpp"
#include "Window/OpenGL_Window.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Events/EventHandler.hpp"
#include "Events/EventListener.hpp"

#if defined(SPH_EXPORTED) && !defined(SPH_BUILD_ENGINE)
	#include "ExportedProjectData.hpp"
	#include "FileUtils/OSPaths.hpp"
	#include "BehaviorCreator.hpp"
#endif


namespace Sharpheus {

#ifndef SPH_BUILD_EXPORTED

	GameBase::GameBase()
	{
	}


	GameBase::~GameBase()
	{
	}

#else

	Game::Game() : GameBase()
	{
		Engine::Init(BehaviorCreator::Instance());

		EventHandler::Init(SPH_BIND(Game::WindowClosed));

		win = new OpenGL_Window();

		proj = new Project(projectData, OSPaths::Get(OSPaths::Folder::EXEC_FOLDER), projectData.name + "proj.sharpheus");
		win->SetProps(proj->GetWinProps());
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

#endif
}
