#include "pch.h"
#include "ResourceManager.hpp"


namespace Sharpheus {

	std::string ResourceManager::assetsRoot;
	std::unordered_map<std::string, Image*> ResourceManager::images;
	std::unordered_map<std::string, Font*> ResourceManager::fontsByPaths;
	std::unordered_map<std::string, Font*> ResourceManager::fontsByNames;
	Image* ResourceManager::circle;


	void ResourceManager::Init(const std::string& projectPath)
	{
		assetsRoot = projectPath + "Assets\\";
		circle = new Image("Shapes\\circle.png", true);
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
		auto it = images.find(path);
		if (it != images.end()) {
			return (*it).second;
		}

		Image* newImage = new Image(path, filtered);
		images[path] = newImage;
		return newImage;
	}


	std::string ResourceManager::Concat(const std::string& first, const std::string& second)
	{
		return first + '|' + second;
	}

}