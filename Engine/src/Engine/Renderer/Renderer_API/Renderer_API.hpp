#pragma once

#include "pch.h"


namespace Sharpheus {

	class Renderer_API
	{
	public:
		virtual ~Renderer_API() = default;

		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void DrawQuad(const Point& begin, const Point& end) = 0;
	};

}