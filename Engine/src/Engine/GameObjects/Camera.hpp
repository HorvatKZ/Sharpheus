#pragma once

#include "pch.h"
#include "GameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT Camera : public GameObject
	{
	public:
		Camera(const std::string& name, const Transform& trafo, float width, float height);
		virtual ~Camera() = default;

		virtual Point& Project(const Point& pos);
		void SetCurrent();

		void WindowResized(const WindowResizedEvent& e);

	protected:
		float width, height;

		virtual void TickThis(float deltaTime) override;
		virtual void RenderThis() override;
	};

}