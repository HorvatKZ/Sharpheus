#pragma once

#include "Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT ResourceManager
	{
	public:
		static void Init();
		static void Clear();

		static void UseSecondary();
		static void UsePrimary();

		static Image* GetImage(const std::string& path, bool filtered = false);
		static inline Image* GetCicrle() { return isPrimary ? circle : circleSecondary; }

	private:
		// Primary
		static std::unordered_map<std::string, Image*> images;
		static Image* circle;

		// Secondary
		static std::unordered_map<std::string, Image*> imagesSecondary;
		static Image* circleSecondary;

		// Other
		static bool isPrimary;

		static void ClearSecondary();
		static Image* GetImageFrom(std::unordered_map<std::string, Image*>& images, const std::string& path, bool filtered);
	};

}