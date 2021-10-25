#include "pch.h"
#include "RenderableListener.hpp"
#include "Engine/Level.hpp"


namespace Sharpheus {

	RenderableListener::~RenderableListener()
	{
		UnSubscribeForRender();
	}


	void RenderableListener::SubscribeForRender(Level* level, const std::string& layerName, std::function<void()>&& func)
	{
		_level = level;
		bool success = level->AddToLayer(listenerID, std::move(func), layerName);
		SPH_ASSERT(success, "Could not subscribe for render");
		isSubscribed = true;
	}


	void RenderableListener::UnSubscribeForRender()
	{
		if (_level != nullptr && isSubscribed) {
			bool success = _level->RemoveFromLayers(listenerID);
			SPH_ASSERT(success, "Could not unsubscribe for render");
			isSubscribed = false;
		}
	}
}