#include "pch.h"
#include "Image.hpp"

#include <stb_image.h>


namespace Sharpheus {

	Point Image::fullTexCoords[] = { Point(0, 0), Point(1, 0), Point(1, 1), Point(0, 1) };

	Image::Image(const std::string& path, bool filtered) : Resource(path), filtered(filtered)
	{
		LoadImg();
	}


	Image::~Image()
	{
		if (ID != TEXTURE_ID_NONE) {
			glDeleteTextures(1, &ID);
		}
	}


	void Image::Render(Point coords[4], const Color& tint)
	{
		glBindTexture(GL_TEXTURE_2D, ID);
		Renderer::DrawQuad(coords, fullTexCoords, tint);
	}


	void Image::RenderPart(Point coords[4], Point texCoords[4], const Color& tint)
	{
		glBindTexture(GL_TEXTURE_2D, ID);
		Renderer::DrawQuad(coords, texCoords, tint);
	}


	void Image::LoadImg()
	{
		SPH_LOG("Importing image: \"{0}\"", fullPath);
		glGenTextures(1, &ID);
		if (ID == TEXTURE_ID_NONE) {
			GLenum err = glGetError();
			SPH_ERROR("Error: {0} - {1}. Could not create texture", err, gluErrorString(err));
		}
		glBindTexture(GL_TEXTURE_2D, ID);
		glActiveTexture(ID);

		int channels;
		unsigned char* data = stbi_load(fullPath.c_str(), (int*)&width, (int*)&height, &channels, 0);
		if (data) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

			GLint sampling = filtered ? GL_LINEAR : GL_NEAREST;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampling);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampling);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			valid = true;
			SPH_LOG("Import successful({0}x{1} {2})", width, height, channels == 4 ? "RGBA" : "RGB");
		} else {
			SPH_ERROR("Unable to import image: \"{0}\"", fullPath);
		}

		stbi_image_free(data);
	}
}