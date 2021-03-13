#include "pch.h"
#include "alma.hpp"


using namespace Sharpheus;


ClassInfo alma::classInfo("alma", "behavior.png", {});


alma::alma(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other)
{
	Init();
}


alma::alma(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name)
{
	Init();
}


alma::~alma()
{
}


void alma::Tick(float deltaTime)
{
	// Called once per tick
}


void alma::Init()
{
	// Do subscribing here
}


bool alma::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

