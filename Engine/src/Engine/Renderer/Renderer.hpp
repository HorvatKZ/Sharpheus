#pragma once

#include "Renderer_API/Renderer_API.hpp"


namespace Sharpheus {

	class SPH_EXPORT Renderer
	{
	public:
		static void Init();
		static void Clear();

		static void StartFrame();
		static void EndFrame();

		static void DrawQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown, const Point& leftDown, const Color& tint);
		static void DrawMonocromeQuad(const Point& leftUp, const Point& rightUp, const Point& rightDown,
			const Point& leftDown, const Color& color);

		static inline class Camera* GetCamera();
		static void SetCamera(class Camera* camera);

		static const Color& GetBackgroundColor();
		static void SetBackgroundColor(const Color& color);

	private:
		static Renderer_API* renderer;
	};

}