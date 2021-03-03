#include "pch.h"
#include "ResourceManager.hpp"


namespace Sharpheus {

	std::string ResourceManager::assetsRoot;
	std::unordered_map<std::string, Image*> ResourceManager::images;
	Image* ResourceManager::circle;


	void ResourceManager::Init(const std::string& projectPath)
	{
		assetsRoot = projectPath + "Assets\\";
		circle = new Image("Shapes\\circle.png", true);
	}


	void ResourceManager::Clear()
	{
		for (std::unordered_map<std::string, Image*>::iterator it = images.begin(); it != images.end(); ++it) {
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

}