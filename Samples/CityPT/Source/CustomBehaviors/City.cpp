#include "pch.h"
#include "City.hpp"


using namespace Sharpheus;


const uint32 City::classVersion = 1;

SPH_START_CLASSINFO(City, "behavior.png")
// Provide necessary data members here for the Editor to present them
SPH_END_CLASSINFO


City::City(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), RenderableListener(GetID()), cityRenderer(*this)
{
}


City::City(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), RenderableListener(GetID()), cityRenderer(*this)
{
}


City::~City()
{
}


bool City::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	// Read data members here
	return fl.GetStatus();
}


bool City::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	// Write data members here
	return fs.GetStatus();
}


void City::Tick(float deltaTime)
{
	// Called once per tick
}


void City::CopyContent(GameObject* other)
{
	Sharpheus::Behavior::CopyContent(other);

	City* trueOther = (City*)other;
	// Copy data members here
}


void City::Init()
{
	SubscribeForRender(level, "Default", SPH_BIND_WITH_0(CityRenderer::Render, &cityRenderer));
}


bool City::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

