#pragma once

#include "pch.h"
#include "Events/WindowEvents.hpp"


namespace Sharpheus {

	class SPH_EXPORT Game
	{
	public:
		Game();
		virtual ~Game();

		void Run();
		void Stop();
		bool IsRunning();

		void WindowClosed(const WindowClosedEvent& e);

	private:
		bool isRunning = true;
		class Window* win;
	};

}