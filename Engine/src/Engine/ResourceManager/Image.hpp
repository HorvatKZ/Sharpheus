#pragma once

#include "Engine/Renderer/Renderer.hpp"
#include <GL/glew.h>
#include <GL/wglew.h>

#define TEXTURE_ID_NONE 0


namespace Sharpheus {

	class SPH_EXPORT Image
	{
	public:
		Image(const std::string& path, bool filtered = false);
		~Image();

		inline uint32_t GetWidth() { return width; }
		inline uint32_t GetHeight() { return height; }
		inline const std::string& GetPath() { return path; }
		inline bool IsFiltered() { return filtered; }


		void Render(const Point& leftUp, const Point& rightUp, const Point& rightDown, const Point& leftDown, const Color& tint = Color::White);

	private:
		GLuint ID = TEXTURE_ID_NONE;
		uint32_t width, height;
		bool filtered;
		std::string path;

		void LoadImg(const std::string& path);
	};

}