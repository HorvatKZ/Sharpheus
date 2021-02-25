#pragma once

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

		void DrawQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown, const Point& leftDown, const Color& tint) override;
		void DrawMonocromeQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown,
			const Point& leftDown, const Color& color) override;

	};

}