#pragma once

#include "Events/WindowEvents.hpp"
#include "Project.hpp"


namespace Sharpheus {

	class SPH_EXPORT GameBase
	{
	public:
		GameBase();
		virtual ~GameBase();

		virtual void WindowClosed(const WindowClosedEvent& e) = 0;

	protected:
		Project* proj = nullptr;
	};

#ifdef SPH_BUILD_EXPORTED
	class Game : public GameBase
	{
	public:
		Game();
		virtual ~Game();

		virtual void Run();
		void Stop();
		bool IsRunning();

		void WindowClosed(const WindowClosedEvent& e) override;

	private:
		bool isRunning = true;
		class Window* win;
	};
#endif
}