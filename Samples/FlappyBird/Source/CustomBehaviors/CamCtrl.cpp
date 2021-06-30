#include "pch.h"
#include "CamCtrl.hpp"
#include "Engine/Level.hpp"
#include "Engine/ProjectControl.hpp"
#include "GameState.hpp"


using namespace Sharpheus;


uint32_t CamCtrl::classVersion = 1;

ClassInfo CamCtrl::classInfo("CamCtrl", "behavior.png", {
	// Provide necessary data members here for the Editor to present them
});


CamCtrl::CamCtrl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ControlListener(GetID())
{
}


CamCtrl::CamCtrl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ControlListener(GetID())
{
}


CamCtrl::~CamCtrl()
{
}


bool CamCtrl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	// Read data members here
	return fl.GetStatus();
}


bool CamCtrl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	// Write data members here
	return fs.GetStatus();
}


void CamCtrl::Tick(float deltaTime)
{
	Transform newTrafo = parent->GetWorldTrafo();
	newTrafo.pos.x = bird->GetWorldTrafo().pos.x;
	parent->SetWorldTrafo(newTrafo);

	uint32_t points = GameState::Instance().GetPoints();
	if (points != prevPoints) {
		scoreText->SetContent("Score: " + std::to_string(points));
		prevPoints = points;
	}

	if (GameState::Instance().IsFailed()) {
		goPanel->SetVisible(true);
		ProjectControl::Pause();
	}
}


void CamCtrl::CopyContent(GameObject* other)
{
	Sharpheus::Behavior::CopyContent(other);

	CamCtrl* trueOther = (CamCtrl*)other;
	// Copy data members here
}


void CamCtrl::OnRetry(const ControlChangedEvent& e)
{
	ProjectControl::LoadLevel("Game.lvl.sharpheus");
}


void CamCtrl::OnBack(const ControlChangedEvent& e)
{
	ProjectControl::LoadLevel("Menu.lvl.sharpheus");
}


void CamCtrl::Init()
{
	bird = level->GetGameObject("Bird");
	scoreText = (Text*)parent->GetChild("Score");
	goPanel = (Quad*)parent->GetChild("GameOverTab");

	SubscribeControlChanged((Control*)parent->GetChild("Back"), SPH_BIND(CamCtrl::OnBack));
	SubscribeControlChanged((Control*)goPanel->GetChild("Retry"), SPH_BIND(CamCtrl::OnRetry));
	SubscribeControlChanged((Control*)goPanel->GetChild("Menu"), SPH_BIND(CamCtrl::OnBack));
}


bool CamCtrl::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

