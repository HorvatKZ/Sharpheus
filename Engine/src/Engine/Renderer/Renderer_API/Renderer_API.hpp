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

		virtual void DrawQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown, const Point& leftDown, const Color& tint) = 0;
		virtual void DrawMonocromeQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown,
			const Point& leftDown, const Color& color) = 0;

	protected:
		Camera* camera = nullptr;
	};

}