#pragma once

#include "Resource.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	class SPH_EXPORT Image : public Resource
	{
	public:
		Image(const std::string& path, bool filtered = false);
		~Image();

		inline uint32 GetWidth() { return width; }
		inline uint32 GetHeight() { return height; }
		inline bool IsFiltered() { return filtered; }

		void Render(Point coords[4], const Color& tint = Color::White);
		void RenderPart(Point coords[4], Point texCoords[4], const Color& tint = Color::White);

	private:
		uint32 ID;
		uint32 width, height;
		bool filtered;

		static Point fullTexCoords[4];

		void LoadImg();
	};

}