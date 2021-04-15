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

		static std::string GetLoadLevelPath();
		static bool NeedToExit();
		static void ResetExit();

	private:
		static std::string loadLevelPath;
		static bool needToExit;
	};

}