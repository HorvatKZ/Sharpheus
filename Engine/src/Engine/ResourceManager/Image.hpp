#pragma once

#include "pch.h"
#include "Engine/Renderer/Renderer.hpp"
#include <GL/glew.h>
#include <GL/wglew.h>

#define TEXTURE_ID_NONE 0


namespace Sharpheus {

	class Image
	{
	public:
		Image(const std::string& path, bool filtered = false);
		~Image();

		int GetWidth();
		int GetHeight();
		std::string GetPath();

		void Render(const Point& begin, const Point& end);

	private:
		GLuint ID = TEXTURE_ID_NONE;
		int width, height;
		bool filtered;
		std::string path;

		void LoadImg(const std::string& path);
	};

}