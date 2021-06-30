#include "pch.h"
#include "UIControl.hpp"
#include "GameState.hpp"
#include "Engine/Level.hpp"
#include "Engine/ProjectControl.hpp"


using namespace Sharpheus;


uint32_t UIControl::classVersion = 1;

ClassInfo UIControl::classInfo("UIControl", "behavior.png", {
	// Provide necessary data members here for the Editor to present them
});


UIControl::UIControl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ControlListener(GetID())
{
}


UIControl::UIControl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ControlListener(GetID())
{
}


UIControl::~UIControl()
{
}


bool UIControl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	// Read data members here
	return fl.GetStatus();
}


bool UIControl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	// Write data members here
	return fs.GetStatus();
}


void UIControl::Tick(float deltaTime)
{
	uint32_t p1Turns = GameState::Instance().GetP1Turns();
	uint32_t p2Turns = GameState::Instance().GetP2Turns();
	p11->SetVisible(p1Turns >= 1);
	p12->SetVisible(p1Turns >= 2);
	p21->SetVisible(p2Turns >= 1);
	p22->SetVisible(p2Turns >= 2);

	p1Points->SetContent(std::to_string(GameState::Instance().GetP1Points()));
	p2Points->SetContent(std::to_string(GameState::Instance().GetP2Points()));
	go->SetVisible(!GameState::Instance().IsInTurn());

	if (GameState::Instance().IsFinished() && !winnerPanel->IsVisible()) {
		winnerPanel->SetVisible(true);
		winnerName->SetContent(GameState::Instance().IsP1TheWinner() ? "Player1" : "Player2");
	}
}

void UIControl::CopyContent(GameObject* other)
{
	Behavior::CopyContent(other);
}


void UIControl::OnBackToMenu(const ControlChangedEvent& e)
{
	ProjectControl::LoadLevel("Menu.lvl.Sharpheus");
}


void UIControl::Init()
{
	p11 = (Sprite*)level->GetGameObject("P1_1");
	p12 = (Sprite*)level->GetGameObject("P1_2");
	p21 = (Sprite*)level->GetGameObject("P2_1");
	p22 = (Sprite*)level->GetGameObject("P2_2");

	p1Points = (Text*)level->GetGameObject("P1_Points");
	p2Points = (Text*)level->GetGameObject("P2_Points");
	go = (Text*)level->GetGameObject("Go");

	winnerPanel = (Quad*)level->GetGameObject("WinnerPanel");
	winnerName = (Text*)level->GetGameObject("DynamicWinnerText");

	SubscribeControlChanged((Control*)level->GetGameObject("WinnerOK"), SPH_BIND(UIControl::OnBackToMenu));
	SubscribeControlChanged((Control*)level->GetGameObject("BackToMenu"), SPH_BIND(UIControl::OnBackToMenu));
}


bool UIControl::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

