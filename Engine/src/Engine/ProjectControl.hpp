#pragma once

#include "pch.h"

namespace Sharpheus {

	class SPH_EXPORT ProjectControl
	{
	public:
		static void Init();
		static void Clear();

		static void LoadLevel(const std::string& path);
		static void Exit();
		static void Pause();
		static void Resume();

		static std::string GetLoadLevelPath();
		static bool NeedToExit();
		static bool IsPaused();

	private:
		static std::string loadLevelPath;
		static bool needToExit;
		static bool isPaused;
	};

}