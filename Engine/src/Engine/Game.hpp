#pragma once

#include "Events/WindowEvents.hpp"
#include "Level.hpp"


namespace Sharpheus {

	class SPH_EXPORT GameBase
	{
	public:
		GameBase();
		virtual ~GameBase();

		virtual void Run();
		void Stop();
		bool IsRunning();

		virtual void WindowClosed(const WindowClosedEvent& e) = 0;

	protected:
		bool isRunning = true;
		class Window* win;

		Level* level;
	};


	class SPH_EXPORT Game : public GameBase
	{
	public:
		Game();
		virtual ~Game();

		void WindowClosed(const WindowClosedEvent& e) override;
	};

}