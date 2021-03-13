#include "pch.h"
#include "asdasd.hpp"


using namespace Sharpheus;


ClassInfo asdasd::classInfo("asdasd", "behavior.png", {});


asdasd::asdasd(Sharpheus::Behavior* other)
	: PlayerContoller(other)
{
	Init();
}


asdasd::asdasd(Sharpheus::GameObject* parent, const std::string& name)
	: PlayerContoller(parent, name)
{
	Init();
}


asdasd::~asdasd()
{
}


void asdasd::Tick(float deltaTime)
{
	// Called once per tick
}


void asdasd::Init()
{
	// Do subscribing here
}


bool asdasd::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

