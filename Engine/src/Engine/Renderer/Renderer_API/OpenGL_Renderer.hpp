#pragma once

#include "pch.h"
#include "Renderer_API.hpp"
#include "Engine/Events/EventListener.hpp"


namespace Sharpheus {

	class OpenGL_Renderer : public Renderer_API, public EventListener
	{
	public:
		OpenGL_Renderer();
		virtual ~OpenGL_Renderer();

		void StartFrame() override;
		void EndFrame() override;

		void DrawQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown, const Point& leftDown) override;

		void ScreenResized(const WindowResizedEvent& e);

	private:
		float screenWidth, screenHeight;
	};

}