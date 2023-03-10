#pragma once

#include "Renderer_API.hpp"


namespace Sharpheus {

	class SPH_EXPORT Renderer
	{
	public:
		static void Init();
		static void Clear();
		static bool IsInited();

		static void StartFrame(const Point& shift = Point());
		static void EndFrame();

		static void UsePrimary();
		static void UseSecondary();

		static void DrawHorizontalLine(float begin, float end, float place, float thickness, const Color& color);
		static void DrawVerticalLine(float begin, float end, float place, float thickness, const Color& color);
		static void DrawLine(const Point& begin, const Point& end, float thickness, const Color& color);

		static void DrawQuad(const Point coords[4], const Point texCoords[4], const Color& tint, uint32 texID);
		static void DrawMonocromeQuad(const Point coords[4], const Color& color);

		static void DrawCircle(const Point coords[4], const Color& tint);
		static void DrawCirclePart(const Point coords[4], const Point texCoords[4], const Color& tint);

		static inline class Camera* GetCamera() { return renderer->GetCamera(); }
		static void SetCamera(class Camera* camera);

		static const Color& GetBackgroundColor();
		static void SetBackgroundColor(const Color& color);

		static uint32 CreateTexture(byte* data, uint32 width, uint32 height, uint32 channels, bool filtered);
		static void FreeTexture(uint32 texID);
		static bool IsValidTexture(uint32 texID);
		static uint32 GetInvalidTexture();

		static inline const Point* const GetFullTexCoords(bool mirrorX = false) { return mirrorX ? fullTexCoordsXMirrored : fullTexCoords; }

		static const std::string& GetAPIVersion();

	private:
		static Renderer_API* renderer;
		static const Point fullTexCoords[4];
		static const Point fullTexCoordsXMirrored[4];
	};

}