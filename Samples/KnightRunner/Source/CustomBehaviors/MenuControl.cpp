#include "pch.h"
#include "MenuControl.hpp"
#include "Engine/ProjectControl.hpp"
#include "GameState.hpp"


using namespace Sharpheus;


uint32_t MenuControl::classVersion = 1;

ClassInfo MenuControl::classInfo("MenuControl", "behavior.png", {
	// Provide necessary data members here for the Editor to present them
});


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
	// Called once per tick
}


void MenuControl::CopyContent(GameObject* other)
{
	Sharpheus::Behavior::CopyContent(other);

	MenuControl* trueOther = (MenuControl*)other;
	// Copy data members here
}

void MenuControl::OnPlay(const ControlChangedEvent& e)
{
	mainPanel->SetVisible(false);
	levelSelect->SetVisible(true);
}


void MenuControl::OnSettings(const ControlChangedEvent& e)
{
	mainPanel->SetVisible(false);
	settingsPanel->SetVisible(true);
}


void MenuControl::OnExit(const ControlChangedEvent& e)
{
	ProjectControl::Exit();
}


void MenuControl::OnBack(const ControlChangedEvent& e)
{
	mainPanel->SetVisible(true);
	levelSelect->SetVisible(false);
	settingsPanel->SetVisible(false);
}


void MenuControl::OnLevel(uint8_t level, const ControlChangedEvent& e)
{
	GameState::Instance().SetCurrLevel(level);
	GameState::Instance().SetDifficulty(diffRadio->GetValue());
	ProjectControl::LoadLevel("level" + std::to_string(level) + ".lvl.sharpheus");
}


void MenuControl::MusicButtonClicked(const ControlChangedEvent& e)
{
	bool state = e.source->GetState();
	GameState::Instance().SetMusic(state);
	if (state) {
		musicPlayer->Play();
	}
	else {
		musicPlayer->Stop();
	}
}


void MenuControl::Init()
{
	mainPanel = parent->GetChild("MainPage");
	levelSelect = parent->GetChild("LevelSelect");
	settingsPanel = parent->GetChild("SettingsPanel");
	diffRadio = (RadioButton*)settingsPanel->GetChild("Easy");
	musicButton = (CheckBox*)settingsPanel->GetChild("Music");
	musicPlayer = (MusicPlayer*)parent->GetChild("MusicP");

	if (GameState::Instance().DoesMusicPlay()) {
		musicButton->SetState(true);
		musicPlayer->Play();
	}
	
	diffRadio->SetValue(GameState::Instance().GetDifficultyStr());

	SubscribeControlChanged((Control*)mainPanel->GetChild("Play"), SPH_BIND(MenuControl::OnPlay));
	SubscribeControlChanged((Control*)mainPanel->GetChild("Settings"), SPH_BIND(MenuControl::OnSettings));
	SubscribeControlChanged((Control*)mainPanel->GetChild("Exit"), SPH_BIND(MenuControl::OnExit));
	SubscribeControlChanged((Control*)levelSelect->GetChild("Level1"), SPH_BIND_THIS_WITH_1(MenuControl::OnLevel, 1));
	SubscribeControlChanged((Control*)levelSelect->GetChild("Level2"), SPH_BIND_THIS_WITH_1(MenuControl::OnLevel, 2));
	SubscribeControlChanged((Control*)levelSelect->GetChild("Level3"), SPH_BIND_THIS_WITH_1(MenuControl::OnLevel, 3));
	SubscribeControlChanged((Control*)levelSelect->GetChild("Back"), SPH_BIND(MenuControl::OnBack));
	SubscribeControlChanged((Control*)settingsPanel->GetChild("Back_1"), SPH_BIND(MenuControl::OnBack));
	SubscribeControlChanged(musicButton.Get(), SPH_BIND(MenuControl::MusicButtonClicked));
}


bool MenuControl::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}

