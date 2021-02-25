#include "pch.h"
#include "ResourceManager.hpp"


namespace Sharpheus {

	std::unordered_map<std::string, Image*> ResourceManager::images;
	Image* ResourceManager::circle;
	std::unordered_map<std::string, Image*> ResourceManager::imagesSecondary;
	Image* ResourceManager::circleSecondary;
	bool ResourceManager::isPrimary = true;


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

		ClearSecondary();
	}


	void ResourceManager::UseSecondary()
	{
		ClearSecondary();
		isPrimary = false;
	}


	void ResourceManager::UsePrimary()
	{
		isPrimary = true;
	}


	Image* ResourceManager::GetImage(const std::string& path, bool filtered)
	{
		return GetImageFrom(isPrimary ? images : imagesSecondary, path, filtered);
	}


	void ResourceManager::ClearSecondary()
	{
		for (auto it = imagesSecondary.begin(); it != imagesSecondary.end(); ++it) {
			delete (*it).second;
		}
		imagesSecondary.clear();
		delete circleSecondary;
		circleSecondary = nullptr;
	}


	Image* ResourceManager::GetImageFrom(std::unordered_map<std::string, Image*>& images, const std::string& path, bool filtered)
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