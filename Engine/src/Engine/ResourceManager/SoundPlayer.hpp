#pragma once

#include "pch.h"

namespace Sharpheus {

	class SPH_EXPORT SoundPlayer
	{
	public:
		static void Init();
		static void Clear();

		static bool Play(const std::string& path, bool loop = false);
		static void Stop(const std::string& path);

		static inline const std::string& GetCurrent() { return curr; }

	private:
		static std::string curr;
	};

}