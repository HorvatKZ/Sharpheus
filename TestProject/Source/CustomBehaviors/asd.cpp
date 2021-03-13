#include "pch.h"
#include "asd.hpp"


using namespace Sharpheus;


ClassInfo asd::classInfo("asd", "behavior.png", {});


asd::asd(Sharpheus::Behavior* other)
	: DebugBehavior(other)
{
	Init();
}


asd::asd(Sharpheus::GameObject* parent, const std::string& name)
	: DebugBehavior(parent, name)
{
	Init();
}


asd::~asd()
{
}


void asd::Tick(float deltaTime)
{
	// Called once per tick
}


void asd::Init()
{
	// Do subscribing here
}


bool asd::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

