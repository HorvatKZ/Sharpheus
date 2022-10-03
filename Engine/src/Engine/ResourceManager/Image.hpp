#pragma once

#include "Resource.hpp"


namespace Sharpheus {

	class SPH_EXPORT Image : public Resource
	{
	public:
		Image(const std::string& path, bool filtered = false);
		~Image();

		inline uint32 GetWidth() const { return width; }
		inline uint32 GetHeight() const { return height; }
		inline bool IsFiltered() const { return filtered; }

		void Render(const Point coords[4], bool mirrorX = false, const Color& tint = Color::White) const;
		void RenderPart(const Point coords[4], const Point texCoords[4], bool mirrorX = false, const Color& tint = Color::White) const;

	private:
		uint32 ID;
		uint32 width, height;
		bool filtered;
		mutable Point temp[4]; // for mirroring X

		void LoadImg();
	};

}