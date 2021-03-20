#pragma once

#include "Resource.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <GL/glew.h>
#include <GL/wglew.h>

#define TEXTURE_ID_NONE 0


namespace Sharpheus {

	class SPH_EXPORT Image : public Resource
	{
	public:
		Image(const std::string& path, bool filtered = false);
		~Image();

		inline uint32_t GetWidth() { return width; }
		inline uint32_t GetHeight() { return height; }
		inline bool IsFiltered() { return filtered; }

		void Render(Point coords[4], const Color& tint = Color::White);
		void RenderPart(Point coords[4], Point texCoords[4], const Color& tint = Color::White);

	private:
		GLuint ID = TEXTURE_ID_NONE;
		uint32_t width, height;
		bool filtered;

		static Point fullTexCoords[4];

		void LoadImg();
	};

}