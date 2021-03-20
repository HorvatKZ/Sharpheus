#pragma once

#include "Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT ResourceManager
	{
	public:
		static void Init(const std::string& projectPath);
		static void Clear();

		static inline const std::string& GetAssetsRoot() { return assetsRoot; }

		static std::string FullPathToPath(const std::string& fullPath);
		static inline std::string PathToFullPath(const std::string& path) { return assetsRoot + path; }

		static Image* GetImage(const std::string& path, bool filtered = false);
		static inline Image* GetCircle() { return circle; }

	private:
		static std::string assetsRoot;
		static std::unordered_map<std::string, Image*> images;
		static Image* circle;
	};

}