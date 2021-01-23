#include "pch.h"
#include "Image.hpp"

#include <stb_image.h>


namespace Sharpheus {

	Image::Image(const std::string& path, bool filtered) : path(path), filtered(filtered)
	{
		LoadImg(path);
	}


	Image::~Image()
	{
		if (ID != TEXTURE_ID_NONE) {
			glDeleteTextures(1, &ID);
		}
	}


	int Image::GetWidth()
	{
		return width;
	}


	int Image::GetHeight()
	{
		return height;
	}


	std::string Image::GetPath()
	{
		return path;
	}


	void Image::Render(const Point& begin, const Point& end)
	{
		glBindTexture(GL_TEXTURE_2D, ID);
		Renderer::DrawQuad(begin, end);
	}


	void Image::LoadImg(const std::string& path)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glActiveTexture(ID);

		int channels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

			GLint sampling = filtered ? GL_LINEAR : GL_NEAREST;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampling);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampling);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		stbi_image_free(data);
	}
}