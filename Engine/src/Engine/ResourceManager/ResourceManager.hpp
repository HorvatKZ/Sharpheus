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
		static void Reload();

		static inline const std::string& GetAssetsRoot() { return assetsRoot; }

		static std::string FullPathToPath(const std::string& fullPath);
		static inline std::string PathToFullPath(const std::string& path) { return assetsRoot + path; }

		static inline std::string GetShaderPath(const std::string& fname) { return assetsRoot + "Shaders/" + fname; }

		static inline std::string GetScriptsRoot() { return scriptRoot; }
		static inline std::string GetScriptPath(const std::string& fname) { return scriptRoot + fname + ".py"; }

		static const Font* GetFont(const std::string& fontFile, const std::string& imgFile);
		static const Font* GetFont(const std::string& fontFile, const Image* img);
		static const Font* GetFont(const std::string& name);
		static inline const std::unordered_map<std::string, Font*>* GetFontTable() { return &fontsByNames; }

		static const Image* GetImage(const std::string& path, bool filtered = false);

		static const Animation* GetAnimation(const std::string& path);

		static const TileSet* GetTileSet(const std::string& path);

		static const Audio* GetAudio(const std::string& path);

		template <typename T>
		static const T* GetResource(const std::string& path);

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


	template<typename T>
	inline const T* ResourceManager::GetResource(const std::string& path)
	{
		static_assert("Not supported resourtce type");
		return nullptr;
	}

	template<>
	inline const Animation* ResourceManager::GetResource<Animation>(const std::string& path)
	{
		return GetAnimation(path);
	}

	template<>
	inline const TileSet* ResourceManager::GetResource<TileSet>(const std::string& path)
	{
		return GetTileSet(path);
	}

	template<>
	inline const Audio* ResourceManager::GetResource<Audio>(const std::string& path)
	{
		return GetAudio(path);
	}

}