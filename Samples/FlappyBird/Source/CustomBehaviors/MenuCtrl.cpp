#include "pch.h"
#include "MenuCtrl.hpp"
#include "Engine/ProjectControl.hpp"


using namespace Sharpheus;


const uint32_t MenuCtrl::classVersion = 1;

SPH_START_CLASSINFO(MenuCtrl, "behaviorcpp.png")
	// Provide necessary data members here for the Editor to present them
SPH_END_CLASSINFO


MenuCtrl::MenuCtrl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ControlListener(GetID())
{
}


MenuCtrl::MenuCtrl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ControlListener(GetID())
{
}


MenuCtrl::~MenuCtrl()
{
}


bool MenuCtrl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	// Read data members here
	return fl.GetStatus();
}


bool MenuCtrl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	// Write data members here
	return fs.GetStatus();
}


void MenuCtrl::Tick(float deltaTime)
{
	// Called once per tick
}


void MenuCtrl::CopyContent(GameObject* other)
{
	Sharpheus::Behavior::CopyContent(other);

	MenuCtrl* trueOther = (MenuCtrl*)other;
	// Copy data members here
}


void MenuCtrl::OnPlay(const Sharpheus::ControlChangedEvent& e)
{
	ProjectControl::LoadLevel("Game.lvl.sharpheus");
}


void MenuCtrl::OnExit(const Sharpheus::ControlChangedEvent& e)
{
	ProjectControl::Exit();
}


void MenuCtrl::Init()
{
	SubscribeControlChanged((Control*)parent->GetChild("Play"), SPH_BIND(MenuCtrl::OnPlay));
	SubscribeControlChanged((Control*)parent->GetChild("Exit"), SPH_BIND(MenuCtrl::OnExit));
}


bool MenuCtrl::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

