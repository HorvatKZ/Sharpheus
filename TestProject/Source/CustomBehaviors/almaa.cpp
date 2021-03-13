#include "pch.h"
#include "almaa.hpp"




using namespace Sharpheus




ClassInfo almaa::classInfo("almaa", "behavior.png", {});




almaa::almaa(Sharpheus::Behavior* other)
	: Behavior(Sharpheus::Behavior* other)
{
	Init()
}




almaa::almaa(Sharpheus::GameObject* parent, const std::string& name)
	: Behavior(Sharpheus::GameObject* parent, const std::string& name)
{
	Init()
}




almaa::~almaa()
{
}




void almaa::Tick(float deltaTime)
	// Called once per tick
}




void almaa::Init()
	// Do subscribing here
}




bool almaa::IsCompatibleWithParent(GameObject* parent)
	return parent != nullptr;
}


