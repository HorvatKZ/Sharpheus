#include "pch.h"
#include "Renderer.hpp"
#include "OpenGL/OGLRenderer.hpp"


namespace Sharpheus {

	Renderer_API* Renderer::renderer = nullptr;
	const Point Renderer::fullTexCoords[] = { Point(0, 0), Point(1, 0), Point(1, 1), Point(0, 1) };


	void Renderer::Init()
	{
		renderer = new OpenGL::Renderer();
	}


	void Renderer::Clear()
	{
		delete renderer;
		renderer = nullptr;
	}


	bool Renderer::IsInited()
	{
		return renderer != nullptr;
	}


	void Renderer::StartFrame(const Point& shift)
	{
		renderer->StartFrame(shift);
	}


	void Renderer::EndFrame()
	{
		renderer->EndFrame();
	}


	void Renderer::UsePrimary()
	{
		renderer->SetSecondary(false);
	}


	void Renderer::UseSecondary()
	{
		renderer->SetSecondary(true);
	}


	void Renderer::DrawHorizontalLine(float begin, float end, float place, float thickness, const Color& color)
	{
		renderer->DrawHorizontalLine(begin, end, place, thickness, color);
	}
	
	
	void Renderer::DrawVerticalLine(float begin, float end, float place, float thickness, const Color& color)
	{
		renderer->DrawVerticalLine(begin, end, place, thickness, color);
	}


	void Renderer::DrawLine(const Point& begin, const Point& end, float thickness, const Color& color)
	{
		renderer->DrawLine(begin, end, thickness, color);
	}


	void Renderer::DrawQuad(const Point coords[4], const Point texCoords[4], const Color& tint, uint32 texID)
	{
		renderer->DrawQuad(coords, texCoords, tint, texID);
	}


	void Renderer::DrawMonocromeQuad(const Point coords[4], const Color& color)
	{
		renderer->DrawMonocromeQuad(coords, color);
	}


	void Renderer::DrawCircle(const Point coords[4], const Color& tint)
	{
		renderer->DrawCircle(coords, tint);
	}


	void Renderer::DrawCirclePart(const Point coords[4], const Point texCoords[4], const Color& tint)
	{
		renderer->DrawCirclePart(coords, texCoords, tint);
	}


	void Renderer::SetCamera(class Camera* camera)
	{
		renderer->SetCamera(camera);
	}

	const Color& Renderer::GetBackgroundColor()
	{
		return renderer->GetBackgroundColor();
	}

	void Renderer::SetBackgroundColor(const Color& color)
	{
		renderer->SetBackgroundColor(color);
	}

	uint32 Renderer::CreateTexture(byte* data, uint32 width, uint32 height, uint32 channels, bool filtered)
	{
		return renderer->CreateTexture(data, width, height, channels, filtered);
	}

	void Renderer::FreeTexture(uint32 texID)
	{
		renderer->FreeTexture(texID);
	}

	bool Renderer::IsValidTexture(uint32 texID)
	{
		return renderer->IsValidTexture(texID);
	}

	uint32 Renderer::GetInvalidTexture()
	{
		return renderer->GetInvalidTexture();
	}

	const std::string& Renderer::GetAPIVersion()
	{
		return renderer->GetVersion();
	}

}