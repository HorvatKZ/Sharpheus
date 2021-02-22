#include "pch.h"
#include "ResourceManager.hpp"


namespace Sharpheus {

	std::unordered_map<std::string, Image*> ResourceManager::images;
	Image* ResourceManager::circle;

	void ResourceManager::Init()
	{
		circle = new Image("../Assets/Shapes/circle.png", true);
	}

	void ResourceManager::Clear()
	{
		for (std::unordered_map<std::string, Image*>::iterator it = images.begin(); it != images.end(); ++it) {
			delete (*it).second;
		}
		delete circle;
	}

	Image* ResourceManager::GetImage(const std::string& path, bool filtered)
	{
		std::unordered_map<std::string, Image*>::iterator it = images.find(path);
		if (it != images.end()) {
			return (*it).second;
		}

		Image* newImage = new Image(path, filtered);
		images[path] = newImage;
		return newImage;
	}

}