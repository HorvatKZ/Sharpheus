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
		SPH_VERIFY(level->AddToLayer(listenerID, std::move(func), layerName), "Could not subscribe for render");
		isSubscribed = true;
	}


	void RenderableListener::UnSubscribeForRender()
	{
		if (_level != nullptr && isSubscribed) {
			SPH_VERIFY(_level->RemoveFromLayers(listenerID), "Could not unsubscribe for render");
			isSubscribed = false;
		}
	}
}