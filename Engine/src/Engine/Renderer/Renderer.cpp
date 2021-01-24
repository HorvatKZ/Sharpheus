#include "pch.h"
#include "Renderer.hpp"

namespace Sharpheus {

	Renderer_API* Renderer::renderer = nullptr;


	void Renderer::Init()
	{
		renderer = new OpenGL_Renderer();
	}


	void Renderer::Clear()
	{
		delete renderer;
	}


	void Renderer::StartFrame()
	{
		renderer->StartFrame();
	}


	void Renderer::EndFrame()
	{
		renderer->EndFrame();
	}


	void Renderer::DrawQuad(const Point& begin, const Point& end)
	{
		renderer->DrawQuad(begin, end);
	}

}