#include "pch.h"
#include "Collection.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(Collection, "collection.png")
	SPH_END_CLASSINFO


	Collection::Collection(GameObject* parent, const std::string& name)
		: GameObject(parent, name)
	{
	}
	

	Collection::~Collection()
	{
	}

}