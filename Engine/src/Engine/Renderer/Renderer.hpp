#pragma once

#include "Renderer_API/Renderer_API.hpp"


namespace Sharpheus {

	class Renderer
	{
	public:
		static void Init();
		static void Clear();

		static void StartFrame();
		static void EndFrame();

		static void DrawQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown, const Point& leftDown);
		static void DrawMonocromeQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown,
			const Point& leftDown, const Color& color);

		static void SetCamera(class Camera* camera);

	private:
		static Renderer_API* renderer;
	};

}