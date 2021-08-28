#pragma once

#include "Renderer_API.hpp"


namespace Sharpheus {

	class SPH_EXPORT Renderer
	{
	public:
		static void Init();
		static void Clear();

		static void StartFrame(const Point& shift = Point());
		static void EndFrame();

		static void DrawHorizontalLine(float begin, float end, float place, float thickness, const Color& color);
		static void DrawVerticalLine(float begin, float end, float place, float thickness, const Color& color);
		static void DrawLine(const Point& begin, const Point& end, float thickness, const Color& color);

		static void DrawQuad(Point coords[4], Point texCoords[4], const Color& tint, uint32 texID);
		static void DrawMonocromeQuad(Point coords[4], const Color& color);

		static inline class Camera* GetCamera() { return renderer->GetCamera(); }
		static void SetCamera(class Camera* camera);

		static const Color& GetBackgroundColor();
		static void SetBackgroundColor(const Color& color);

		static uint32 CreateTexture(byte* data, uint32 width, uint32 height, uint32 channels, bool filtered);
		static void FreeTexture(uint32 texID);
		static bool IsValidTexture(uint32 texID);
		static uint32 GetInvalidTexture();

		static const std::string& GetAPIVersion();

	private:
		static Renderer_API* renderer;
	};

}