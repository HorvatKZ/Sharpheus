#include "pch.h"
#include "ResourceManager.hpp"


namespace Sharpheus {

	std::string ResourceManager::assetsRoot;
	std::string ResourceManager::scriptRoot;
	std::unordered_map<std::string, Image*> ResourceManager::images;
	std::unordered_map<std::string, Font*> ResourceManager::fontsByPaths;
	std::unordered_map<std::string, Font*> ResourceManager::fontsByNames;
	std::unordered_map<std::string, Animation*> ResourceManager::animations;
	std::unordered_map<std::string, TileSet*> ResourceManager::tileSets;
	std::unordered_map<std::string, Audio*> ResourceManager::audios;


	void ResourceManager::Init(const std::string& projectPath)
	{
		assetsRoot = projectPath + "Assets\\";
		scriptRoot = projectPath + "Scripts\\";
	}


	void ResourceManager::Clear()
	{
		for (auto it = images.begin(); it != images.end(); ++it) {
			delete (*it).second;
		}
		for (auto it = fontsByPaths.begin(); it != fontsByPaths.end(); ++it) {
			delete (*it).second;
		}
		for (auto it = animations.begin(); it != animations.end(); ++it) {
			delete (*it).second;
		}
		for (auto it = tileSets.begin(); it != tileSets.end(); ++it) {
			delete (*it).second;
		}
		for (auto it = audios.begin(); it != audios.end(); ++it) {
			delete (*it).second;
		}

		images.clear();
		fontsByPaths.clear();
		fontsByNames.clear();
		animations.clear();
		tileSets.clear();
		audios.clear();
	}


	void ResourceManager::Reload()
	{
		Clear();
	}


	std::string ResourceManager::FullPathToPath(const std::string& fullPath)
	{
		if (fullPath.length() > assetsRoot.length() && fullPath.substr(0, assetsRoot.length()) == assetsRoot) {
			return fullPath.substr(assetsRoot.length());
		}

		return "";
	}


	const Font* ResourceManager::GetFont(const std::string& fontFile, const std::string& imgFile)
	{
		std::string concated = Concat(fontFile, imgFile);
		auto it = fontsByPaths.find(concated);
		if (it != fontsByPaths.end()) {
			return (*it).second;
		}

		const Image* fontImg = GetImage(imgFile, true);
		Font* newFont = new Font(fontFile, fontImg);
		fontsByPaths[concated] = newFont;
		fontsByNames[newFont->GetName()] = newFont;
		return newFont;
	}


	const Font* ResourceManager::GetFont(const std::string& fontFile, const Image* img)
	{
		return GetFont(fontFile, img->GetPath());
	}


	const Font* ResourceManager::GetFont(const std::string& name)
	{
		auto it = fontsByNames.find(name);
		if (it != fontsByNames.end()) {
			return (*it).second;
		}

		return nullptr;
	}


	const Image* ResourceManager::GetImage(const std::string& path, bool filtered)
	{
		std::string extPath = path + (filtered ? "_f" : "");
		auto it = images.find(extPath);
		if (it != images.end()) {
			return (*it).second;
		}

		Image* newImage = new Image(path, filtered);
		images[extPath] = newImage;
		return newImage;
	}


	const Animation* ResourceManager::GetAnimation(const std::string& path)
	{
		auto it = animations.find(path);
		if (it != animations.end()) {
			return (*it).second;
		}

		Animation* newAnim = new Animation(path);
		animations[path] = newAnim;
		return newAnim;
	}


	const TileSet* ResourceManager::GetTileSet(const std::string& path)
	{
		auto it = tileSets.find(path);
		if (it != tileSets.end()) {
			return (*it).second;
		}

		TileSet* newTileSet = new TileSet(path);
		tileSets[path] = newTileSet;
		return newTileSet;
	}


	const Audio* ResourceManager::GetAudio(const std::string& path)
	{
		auto it = audios.find(path);
		if (it != audios.end()) {
			return (*it).second;
		}

		Audio* newAudio = new Audio(path);
		audios[path] = newAudio;
		return newAudio;
	}


	std::string ResourceManager::Concat(const std::string& first, const std::string& second)
	{
		return first + '|' + second;
	}

}