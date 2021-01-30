#pragma once

#include "pch.h"


namespace Sharpheus {

	class SPH_EXPORT Level
	{
	public:
		Level();
		virtual ~Level();

		void Tick(float deltaTime);
		void Render();

	private:
		class GameObject* root;
	};

}