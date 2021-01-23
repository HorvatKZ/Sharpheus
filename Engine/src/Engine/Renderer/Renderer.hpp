#pragma once

#include "pch.h"
#include "Renderer_API/OpenGL_Renderer.hpp"


namespace Sharpheus {

	class Renderer
	{
	public:
		static void Init();
		static void Clear();

		static void StartFrame();
		static void EndFrame();

		static void DrawQuad(const Point& begin, const Point& end);

	private:
		static Renderer_API* renderer;
	};

}