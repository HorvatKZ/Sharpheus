#include "pch.h"
#include "Game.hpp"

#include "Window/OpenGL_Window.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Events/EventHandler.hpp"
#include "Events/EventListener.hpp"


namespace Sharpheus {

	class WinEventListener : public EventListener // Only for testing
	{
	public:
		WinEventListener() {
			Subscribe<WindowClosedEvent>(SPH_BIND(WinEventListener::WinClosed));
			Subscribe<WindowResizedEvent>(SPH_BIND(WinEventListener::WinResized));
		}

		void WinClosed(const WindowClosedEvent& e) {
			SPH_INFO("{0}", e.ToStr());
		}

		void WinResized(const WindowResizedEvent& e) {
			SPH_INFO("{0}", e.ToStr());
		}
	};

	Game::Game()
	{
		Logger::Init();
		SPH_INFO("Welcome to Sharpheus!");

		ResourceManager::Init();
		EventHandler::Init(SPH_BIND(Game::WindowClosed));

		win = new OpenGL_Window();
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
		Image* image = ResourceManager::GetImage("../Assets/Branding/sharpheus_promo.png", true);
		WinEventListener we;
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


	void Game::WindowClosed(const WindowClosedEvent& e)
	{
		Stop();
	}

} // namespace Sharpheus
