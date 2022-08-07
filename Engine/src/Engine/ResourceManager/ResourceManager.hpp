#pragma once

#include "Image.hpp"
#include "Font.hpp"
#include "Animation.hpp"
#include "TileSet.hpp"
#include "Audio.hpp"


namespace Sharpheus {

	class SPH_EXPORT ResourceManager
	{
	public:
		static void Init(const std::string& projectPath);
		static void Clear();

		static inline const std::string& GetAssetsRoot() { return assetsRoot; }

		static std::string FullPathToPath(const std::string& fullPath);
		static inline std::string PathToFullPath(const std::string& path) { return assetsRoot + path; }

		static inline std::string GetShaderPath(const std::string& fname) { return assetsRoot + "Shaders/" + fname; }

		static inline std::string GetScriptPath(const std::string& fname) { return scriptRoot + fname + ".py"; }

		static Font* GetFont(const std::string& fontFile, const std::string& imgFile);
		static Font* GetFont(const std::string& fontFile, Image* img);
		static Font* GetFont(const std::string& name);
		static inline const std::unordered_map<std::string, Font*>* GetFontTable() { return &fontsByNames; }

		static Image* GetImage(const std::string& path, bool filtered = false);

		static Animation* GetAnimation(const std::string& path);

		static TileSet* GetTileSet(const std::string& path);

		static Audio* GetAudio(const std::string& path);

	private:
		static std::string assetsRoot;
		static std::string scriptRoot;
		static std::unordered_map<std::string, Image*> images;
		static std::unordered_map<std::string, Font*> fontsByPaths;
		static std::unordered_map<std::string, Font*> fontsByNames;
		static std::unordered_map<std::string, Animation*> animations;
		static std::unordered_map<std::string, TileSet*> tileSets;
		static std::unordered_map<std::string, Audio*> audios;

		static std::string Concat(const std::string& first, const std::string& second);
	};

}