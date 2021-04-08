#include "pch.h"
#include "Resource.hpp"
#include "ResourceManager.hpp"


namespace Sharpheus {

	Resource::Resource()
	{
	}


	Resource::Resource(const std::string& path) : path(path)
	{
		fullPath = ResourceManager::PathToFullPath(path);
	}


	Resource::~Resource()
	{
	}


	void Resource::SetPath(const std::string& path)
	{
		this->path = path;
		fullPath = ResourceManager::PathToFullPath(path);
	}

}