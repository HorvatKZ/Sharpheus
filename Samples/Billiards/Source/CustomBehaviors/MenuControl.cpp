#include "pch.h"
#include "MenuControl.hpp"
#include "Engine/Level.hpp"
#include "Engine/ProjectControl.hpp"


using namespace Sharpheus;


const uint32_t MenuControl::classVersion = 1;

SPH_START_CLASSINFO(MenuControl, "behaviorcpp.png")
	// Provide necessary data members here for the Editor to present them
SPH_END_CLASSINFO


MenuControl::MenuControl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ControlListener(GetID())
{
}


MenuControl::MenuControl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ControlListener(GetID())
{
}


MenuControl::~MenuControl()
{
}


bool MenuControl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	// Read data members here
	return fl.GetStatus();
}


bool MenuControl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	// Write data members here
	return fs.GetStatus();
}


void MenuControl::Tick(float deltaTime)
{
	SubscribeControlChanged((Control*)level->GetGameObject("Play"), SPH_BIND(MenuControl::OnPlay));
	SubscribeControlChanged((Control*)level->GetGameObject("Exit"), SPH_BIND(MenuControl::OnExit));
}


void MenuControl::CopyContent(GameObject* other)
{
	Behavior::CopyContent(other);
}


void MenuControl::OnPlay(const Sharpheus::ControlChangedEvent& e)
{
	ProjectControl::LoadLevel("Game.lvl.sharpheus");
}


void MenuControl::OnExit(const Sharpheus::ControlChangedEvent& e)
{
	ProjectControl::Exit();
}


void MenuControl::Init()
{
	// Do subscribing here
}


bool MenuControl::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

