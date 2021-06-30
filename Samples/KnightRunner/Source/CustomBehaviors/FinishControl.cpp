#include "pch.h"
#include "FinishControl.hpp"
#include "GameState.hpp"


using namespace Sharpheus;


uint32_t FinishControl::classVersion = 1;

ClassInfo FinishControl::classInfo("FinishControl", "behavior.png", {
	// Provide necessary data members here for the Editor to present them
});


FinishControl::FinishControl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ColliderListener(GetID())
{
}


FinishControl::FinishControl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ColliderListener(GetID())
{
}


FinishControl::~FinishControl()
{
}


bool FinishControl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	// Read data members here
	return fl.GetStatus();
}


bool FinishControl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	// Write data members here
	return fs.GetStatus();
}


void FinishControl::Tick(float deltaTime)
{
	// Called once per tick
}


void FinishControl::CopyContent(GameObject* other)
{
	Sharpheus::Behavior::CopyContent(other);

	FinishControl* trueOther = (FinishControl*)other;
	// Copy data members here
}

void FinishControl::OnTrigger(const Sharpheus::OnEnterEvent& e)
{
	if (e.obj->GetName() == "Player") {
		GameState::Instance().ReachFinish();
	}
}


void FinishControl::Init()
{
	SubscribeOnTriggerEnter((Collider*)parent->GetFirstChildOfMask(TypeMasks::Collider), SPH_BIND(FinishControl::OnTrigger));
}


bool FinishControl::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

