#pragma once

#include "Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT ResourceManager
	{
	public:
		static void Init();
		static void Clear();

		static Image* GetImage(const std::string& path, bool filtered = false);
		static inline Image* GetCicrle() { return circle; }


	private:
		static std::unordered_map<std::string, Image*> images;
		static Image* circle;
	};

}