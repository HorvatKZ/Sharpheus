#pragma once

#include "Image.hpp"
#include "Font.hpp"


namespace Sharpheus {

	class SPH_EXPORT ResourceManager
	{
	public:
		static void Init(const std::string& projectPath);
		static void Clear();

		static inline const std::string& GetAssetsRoot() { return assetsRoot; }

		static std::string FullPathToPath(const std::string& fullPath);
		static inline std::string PathToFullPath(const std::string& path) { return assetsRoot + path; }

		static Font* GetFont(const std::string& fontFile, const std::string& imgFile);
		static Font* GetFont(const std::string& fontFile, Image* img);
		static Font* GetFont(const std::string& name);
		static inline const std::unordered_map<std::string, Font*>* GetFontTable() { return &fontsByNames; }

		static Image* GetImage(const std::string& path, bool filtered = false);
		static inline Image* GetCircle() { return circle; }

	private:
		static std::string assetsRoot;
		static std::unordered_map<std::string, Image*> images;
		static std::unordered_map<std::string, Font*> fontsByPaths;
		static std::unordered_map<std::string, Font*> fontsByNames;
		static Image* circle;

		static std::string Concat(const std::string& first, const std::string& second);
	};

}