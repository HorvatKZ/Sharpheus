#pragma once

#include "Engine/GameObjects/Camera.hpp"


namespace Sharpheus {

	class Renderer_API
	{
	public:
		virtual ~Renderer_API() = default;

		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

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
		virtual void DrawQuad(Point coords[4], Point texCoords[4], const Color& tint) = 0;
		virtual void DrawMonocromeQuad(Point coords[4], const Color& color) = 0;

	protected:
		Camera* camera = nullptr;
		Color bgColor;
	};

}