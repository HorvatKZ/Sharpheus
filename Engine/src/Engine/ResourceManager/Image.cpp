#include "pch.h"
#include "Image.hpp"

#include <stb_image.h>


namespace Sharpheus {

	Image::Image(const std::string& path, bool filtered) : Resource(path), filtered(filtered), ID(Renderer::GetInvalidTexture())
	{
		LoadImg();
	}


	Image::~Image()
	{
		Renderer::FreeTexture(ID);
	}


	void Image::Render(Point coords[4], const Color& tint)
	{
		Renderer::DrawQuad(coords, Renderer::GetFullTexCoords(), tint, ID);
	}


	void Image::RenderPart(Point coords[4], Point texCoords[4], const Color& tint)
	{
		Renderer::DrawQuad(coords, texCoords, tint, ID);
	}


	void Image::LoadImg()
	{
		SPH_LOG("Importing image: \"{0}\"", fullPath);

		int channels;
		unsigned char* data = stbi_load(fullPath.c_str(), (int*)&width, (int*)&height, &channels, 0);
		if (data != nullptr) {
			ID = Renderer::CreateTexture(data, width, height, channels, filtered);
			valid = Renderer::IsValidTexture(ID);
		}
		
		if (valid) {
			SPH_LOG("Import successful({0}x{1} {2})", width, height, channels == 4 ? "RGBA" : "RGB");
		} else {
			SPH_ERROR("Unable to import image: \"{0}\"", fullPath);
		}

		stbi_image_free(data);
	}
}