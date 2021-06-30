#include "pch.h"
#include "Game.hpp"

#include "ProjectControl.hpp"
#include "Window/OpenGL_Window.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Events/EventHandler.hpp"
#include "Events/EventListener.hpp"
#include "ResourceManager/SoundPlayer.hpp"

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
		Logger::Init();
		SPH_INFO("Welcome to Sharpheus!");

		EventHandler::Init(SPH_BIND(Game::WindowClosed));

		win = new OpenGL_Window();
		SoundPlayer::Init();

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
		SoundPlayer::Clear();

		SPH_INFO("Game successfully exited");
		Logger::Clear();
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
