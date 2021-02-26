#include "pch.h"
#include "Collection.hpp"


namespace Sharpheus {

	ClassInfo Collection::classInfo("Collection", "collection.png", {});


	SPH_IMPL_COPY(Collection);

	Collection::Collection(GameObject* parent, const std::string& name)
		: GameObject(parent, name)
	{
	}
	

	Collection::~Collection()
	{
	}

}