#include "pch.h"
#include "ResourceManager.hpp"


namespace Sharpheus {

	std::string ResourceManager::assetsRoot;
	std::unordered_map<std::string, Image*> ResourceManager::images;
	std::unordered_map<std::string, Font*> ResourceManager::fontsByPaths;
	std::unordered_map<std::string, Font*> ResourceManager::fontsByNames;
	std::unordered_map<std::string, Animation*> ResourceManager::animations;
	std::unordered_map<std::string, TileSet*> ResourceManager::tileSets;
	Image* ResourceManager::circle = nullptr;


	void ResourceManager::Init(const std::string& projectPath)
	{
		assetsRoot = projectPath + "Assets\\";
		if (circle == nullptr) {
			circle = new Image("Shapes\\circle.png", true);
		}
	}


	void ResourceManager::Clear()
	{
		for (auto it = images.begin(); it != images.end(); ++it) {
			delete (*it).second;
		}
		for (auto it = fontsByPaths.begin(); it != fontsByPaths.end(); ++it) {
			delete (*it).second;
		}
		delete circle;
		circle = nullptr;
	}


	std::string ResourceManager::FullPathToPath(const std::string& fullPath)
	{
		if (fullPath.length() > assetsRoot.length() && fullPath.substr(0, assetsRoot.length()) == assetsRoot) {
			return fullPath.substr(assetsRoot.length());
		}

		return "";
	}


	Font* ResourceManager::GetFont(const std::string& fontFile, const std::string& imgFile)
	{
		std::string concated = Concat(fontFile, imgFile);
		auto it = fontsByPaths.find(concated);
		if (it != fontsByPaths.end()) {
			return (*it).second;
		}

		Image* fontImg = GetImage(imgFile, true);
		Font* newFont = new Font(fontFile, fontImg);
		fontsByPaths[concated] = newFont;
		fontsByNames[newFont->GetName()] = newFont;
		return newFont;
	}


	Font* ResourceManager::GetFont(const std::string& fontFile, Image* img)
	{
		return GetFont(fontFile, img->GetPath());
	}


	Font* ResourceManager::GetFont(const std::string& name)
	{
		auto it = fontsByNames.find(name);
		if (it != fontsByNames.end()) {
			return (*it).second;
		}

		return nullptr;
	}


	Image* ResourceManager::GetImage(const std::string& path, bool filtered)
	{
		std::string extPath = path + (filtered ? "_f" : "");
		auto it = images.find(extPath);
		if (it != images.end()) {
			return (*it).second;
		}

		Image* newImage = new Image(path, filtered);
		if (newImage->IsValid()) {
			images[extPath] = newImage;
		}
		return newImage;
	}


	Animation* ResourceManager::GetAnimation(const std::string& path)
	{
		auto it = animations.find(path);
		if (it != animations.end()) {
			return (*it).second;
		}

		Animation* newAnim = new Animation(path);
		animations[path] = newAnim;
		return newAnim;
	}


	TileSet* ResourceManager::GetTileSet(const std::string& path)
	{
		auto it = tileSets.find(path);
		if (it != tileSets.end()) {
			return (*it).second;
		}

		TileSet* newTileSet = new TileSet(path);
		tileSets[path] = newTileSet;
		return newTileSet;
	}


	std::string ResourceManager::Concat(const std::string& first, const std::string& second)
	{
		return first + '|' + second;
	}

}