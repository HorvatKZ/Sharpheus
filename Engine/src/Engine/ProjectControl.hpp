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
		static inline bool NeedToExit() { return needToExit; }
		static inline bool IsPaused() { return isPaused; }

	private:
		static std::string loadLevelPath;
		static bool needToExit;
		static bool isPaused;
	};

}