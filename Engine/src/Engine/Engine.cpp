#include "pch.h"
#include "Engine.hpp"
#include "EngineVersion.hpp"
#include "Level.hpp"
#include "Engine/BehaviorCreatorBase.hpp"
#include "Engine/Logger/Logger.hpp"
#include "Engine/ResourceManager/AudioPlayer.hpp"
#include "Engine/Renderer/Renderer.hpp"

namespace Sharpheus {

	void Engine::Init(BehaviorCreatorBase* bc)
	{
		Logger::Init();
		AudioPlayer::Init();
		Level::SetBehaviorCreator(bc);
		SPH_INFO("Sharpheus Game Engine v." + EngineVersion::latest.GetVName() + " initialized");
	}

	void Engine::Clear()
	{
		AudioPlayer::Clear();
		if (Renderer::IsInited()) {
			Renderer::Clear();
		}

		SPH_INFO("Sharpheus Game Engine v." + EngineVersion::latest.GetVName() + " cleared");
		Logger::Clear();
	}

}