#pragma once

#include "Image.hpp"


namespace Sharpheus {

	class ResourceManager
	{
	public:
		static void Init();
		static void Clear();

		static Image* GetImage(const std::string& path, bool filtered = false);

	private:
		static std::unordered_map<std::string, Image*> images;
	};

}