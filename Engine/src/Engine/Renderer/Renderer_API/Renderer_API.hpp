#pragma once


namespace Sharpheus {

	class Renderer_API
	{
	public:
		virtual ~Renderer_API() = default;

		virtual void StartFrame() = 0;
		virtual void EndFrame() = 0;

		void SetCamera(class Camera* camera) { this->camera = camera; }

		virtual void DrawQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown, const Point& leftDown) = 0;
		virtual void DrawMonocromeQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown,
			const Point& leftDown, const Color& color) = 0;

	protected:
		class Camera* camera = nullptr;
	};

}