#include "pch.h"
#include "Controller.hpp"
#include "Engine/ProjectControl.hpp"


using namespace Sharpheus;


const uint32 Controller::classVersion = 1;

SPH_START_CLASSINFO(Controller, "behavior.png")
// Provide necessary data members here for the Editor to present them
SPH_END_CLASSINFO


Controller::Controller(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ControlListener(GetID())
{
}


Controller::Controller(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ControlListener(GetID())
{
}


Controller::~Controller()
{
}


bool Controller::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	// Read data members here
	return fl.GetStatus();
}


bool Controller::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	// Write data members here
	return fs.GetStatus();
}


void Controller::Tick(float deltaTime)
{
	// Called once per tick
}


void Controller::CopyContent(GameObject* other)
{
	Sharpheus::Behavior::CopyContent(other);

	Controller* trueOther = (Controller*)other;
	// Copy data members here
}


void Controller::OnButtonPress(const Sharpheus::ControlChangedEvent& e)
{
	ProjectControl::LoadLevel("Performance/" + e.source->GetName() + ".lvl.sharpheus");
}


void Controller::Init()
{
	auto buttons = parent->GetChild("Buttons")->GetChildren();
	for (uint32 i = 0; i < buttons.size(); ++i) {
		SubscribeControlChanged((Control*)(buttons[i]), SPH_BIND(Controller::OnButtonPress));
	}
}


bool Controller::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

