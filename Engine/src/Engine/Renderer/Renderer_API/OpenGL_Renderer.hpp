#pragma once

#include "pch.h"
#include "Renderer_API.hpp"


namespace Sharpheus {

	class OpenGL_Renderer : public Renderer_API
	{
	public:
		OpenGL_Renderer();
		virtual ~OpenGL_Renderer();

		void StartFrame() override;
		void EndFrame() override;

		void DrawQuad(const Point& begin, const Point& end) override;
	};

}