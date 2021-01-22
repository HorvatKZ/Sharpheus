#pragma once

#include "pch.h"


namespace Sharpheus {

	class SPH_EXPORT Game
	{
	public:
		Game();
		virtual ~Game();

		void Run();
		void Stop();
		bool IsRunning();

	private:
		bool isRunning = true;
		class Window* win;
	};

}