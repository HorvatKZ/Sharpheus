#include "pch.h"
#include "Renderer.hpp"
#include "Renderer_API/OpenGL_Renderer.hpp"


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


	void Renderer::DrawQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown, const Point& leftDown)
	{
		renderer->DrawQuad(leftUp, rightUp, rightDown, leftDown);
	}

	void Renderer::DrawMonocromeQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown,
		const Point& leftDown, const Color& color)
	{
		renderer->DrawMonocromeQuad(leftUp, rightUp, rightDown, leftDown, color);
	}

	void Renderer::SetCamera(class Camera* camera)
	{
		renderer->SetCamera(camera);
	}

}