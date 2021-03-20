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

		void DrawHorizontalLine(float begin, float end, float place, float thickness, const Color& color) override;
		void DrawVerticalLine(float begin, float end, float place, float thickness, const Color& color) override;
		void DrawQuad(Point coords[4], Point texCoords[4], const Color& tint) override;
		void DrawMonocromeQuad(Point coords[4], const Color& color) override;

	};

}