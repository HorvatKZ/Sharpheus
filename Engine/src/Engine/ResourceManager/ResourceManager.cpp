#include "pch.h"
#include "ResourceManager.hpp"


namespace Sharpheus {

	std::unordered_map<std::string, Image*> ResourceManager::images;

	void ResourceManager::Init()
	{
	}

	void ResourceManager::Clear()
	{
		for (std::unordered_map<std::string, Image*>::iterator it = images.begin(); it != images.end(); ++it) {
			delete (*it).second;
		}
	}

	Image* ResourceManager::GetImage(const std::string& path)
	{
		std::unordered_map<std::string, Image*>::iterator it = images.find(path);
		if (it != images.end()) {
			return (*it).second;
		}

		Image* newImage = new Image(path);
		images[path] = newImage;
		return newImage;
	}

}