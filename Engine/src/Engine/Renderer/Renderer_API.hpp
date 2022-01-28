#pragma once

#include "Engine/GameObjects/Camera.hpp"


namespace Sharpheus {

	class Renderer_API
	{
	public:
		Renderer_API() = default;
		virtual ~Renderer_API() = default;

		virtual void StartFrame(const Point& shift) = 0;
		virtual void EndFrame() = 0;

		virtual void SetSecondary(bool isSecondary) = 0;

		inline Camera* GetCamera() { return camera; }
		void SetCamera(Camera* camera) {
			if (this->camera != nullptr && this->camera != camera) {
				this->camera->SetCurrent(false);
			}
			this->camera = camera;
		}

		inline const Color& GetBackgroundColor() { return bgColor; }
		inline void SetBackgroundColor(const Color& color) { bgColor = color; }

		virtual void DrawHorizontalLine(float begin, float end, float place, float thickness, const Color& color) = 0;
		virtual void DrawVerticalLine(float begin, float end, float place, float thickness, const Color& color) = 0;
		virtual void DrawLine(const Point& begin, const Point& end, float thickness, const Color& color) = 0;

		virtual void DrawQuad(const Point coords[4], const Point texCoords[4], const Color& tint, uint32 texID) = 0;
		virtual void DrawMonocromeQuad(const Point coords[4], const Color& color) = 0;

		virtual void DrawCircle(const Point coords[4], const Color& tint) = 0;
		virtual void DrawCirclePart(const Point coords[4], const Point texCoords[4], const Color& tint) = 0;

		virtual uint32 CreateTexture(byte* data, uint32 width, uint32 height, uint32 channels, bool filtered) = 0;
		virtual void FreeTexture(uint32 texID) = 0;
		virtual bool IsValidTexture(uint32 texID) = 0;
		virtual uint32 GetInvalidTexture() = 0;

		virtual inline const std::string& GetVersion() = 0;

	protected:
		Camera* camera = nullptr;
		Color bgColor;
	};

}