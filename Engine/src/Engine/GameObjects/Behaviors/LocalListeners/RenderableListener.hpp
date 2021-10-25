#pragma once

#include "../Controls/Control.hpp"


namespace Sharpheus {

	class SPH_EXPORT RenderableListener
	{
	public:
		RenderableListener(ID listenerID) : listenerID(listenerID) {}
		virtual ~RenderableListener();

		void SubscribeForRender(class Level* level, const std::string& layerName, std::function<void()>&& func);
		void UnSubscribeForRender();

	protected:
		ID listenerID;
		class Level* _level = nullptr;
		bool isSubscribed = false;
	};

}