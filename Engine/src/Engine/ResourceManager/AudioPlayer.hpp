#pragma once

#include "pch.h"
#include "Audio.hpp"
#include <soloud.h>

namespace Sharpheus {

	class SPH_EXPORT AudioPlayer
	{
	public:
		static void Init();
		static void Clear();

		static SoLoud::handle Play(const Audio* audio, bool looping = false, uint32 volume = 100, float playSpeed = 1.f);
		static void Stop(SoLoud::handle handle);
		static void StopAll();

		static void Pause(SoLoud::handle handle);
		static void Resume(SoLoud::handle handle);
		static inline bool IsPaused(SoLoud::handle handle) { return soundEngine.getPause(handle); }

		static void SetVolume(SoLoud::handle handle, uint32 volume);
		static void SetPlaySpeed(SoLoud::handle handle, float speed);

	private:
		static SoLoud::Soloud soundEngine;
	};

}