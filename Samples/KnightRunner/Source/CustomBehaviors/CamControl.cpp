#include "pch.h"
#include "CamControl.hpp"
#include "Engine/Level.hpp"
#include "Engine/ProjectControl.hpp"


using namespace Sharpheus;


uint32_t CamControl::classVersion = 1;

ClassInfo CamControl::classInfo("CamControl", "behavior.png", {
	// Provide necessary data members here for the Editor to present them
});


CamControl::CamControl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ControlListener(GetID())
{
}


CamControl::CamControl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ControlListener(GetID())
{
}


CamControl::~CamControl()
{
}


bool CamControl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	// Read data members here
	return fl.GetStatus();
}


bool CamControl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	// Write data members here
	return fs.GetStatus();
}


void CamControl::Tick(float deltaTime)
{
	if (player) {
		Transform trafo = parent->GetWorldTrafo();
		trafo.pos.x = player->GetWorldTrafo().pos.x;
		parent->SetWorldTrafo(trafo);

		uint32_t playerHP = player->GetHP();
		if (playerHP != prevHP) {
			hpText->SetContent(std::to_string(playerHP) + "/" + std::to_string(maxHP));
			
			float barWidth = hpBar->GetWidth();
			float ratio = (float)playerHP / maxHP;
			hpBar->SetTrafo(Transform(Point(-barWidth / 2 * (1 - ratio), 0), Point(ratio, 1), 0));

			prevHP = playerHP;
		}

		if (playerHP == 0) {
			GameOver();
		}

		if (GameState::Instance().FinishReached()) {
			Finish();
		}
	}
}


void CamControl::CopyContent(GameObject* other)
{
	Behavior::CopyContent(other);
}


void CamControl::OnPauseButtonPressed(const ControlChangedEvent& e)
{
	if (canUseButtons) {
		if (e.source->GetState()) {
			ProjectControl::Pause();
			pausePanel->SetVisible(true);
		}
		else {
			ProjectControl::Resume();
			pausePanel->SetVisible(false);
		}
	}
}


void CamControl::OnMusicButtonPressed(const Sharpheus::ControlChangedEvent& e)
{
	if (canUseButtons) {
		bool state = e.source->GetState();
		GameState::Instance().SetMusic(state);
		if (state) {
			musicPlayer->Play();
		} else {
			musicPlayer->Stop();
		}
	}
}


void CamControl::OnResume(const ControlChangedEvent& e)
{
	ProjectControl::Resume();
	pausePanel->SetVisible(false);
	pauseButton->SetState(false);
}


void CamControl::OnRetry(const ControlChangedEvent& e)
{
	ProjectControl::LoadLevel(level->GetPath());
}


void CamControl::OnMenu(const ControlChangedEvent& e)
{
	ProjectControl::LoadLevel("Menu.lvl.sharpheus");
}


void CamControl::OnNext(const ControlChangedEvent& e)
{
	uint8_t level = GameState::Instance().GetCurrLevel();
	++level;
	GameState::Instance().SetCurrLevel(level);
	ProjectControl::LoadLevel("Level" + std::to_string(level) + ".lvl.sharpheus");
}


void CamControl::GameOver()
{
	canUseButtons = false;
	ProjectControl::Pause();
	goPanel->SetVisible(true);
}


void CamControl::Finish()
{
	canUseButtons = false;
	ProjectControl::Pause();
	finishPanel->SetVisible(true);
}


void CamControl::Init()
{
	player = (PlayerController*)level->GetGameObject("PlayerController");
	hpBar = (Quad*)level->GetGameObject("HP_Green");
	hpText = (Text*)level->GetGameObject("HP_Text");

	pausePanel = (Quad*)parent->GetChild("PauseMenu");
	finishPanel = (Quad*)parent->GetChild("FinishMenu");
	goPanel = (Quad*)parent->GetChild("GameOverMenu");
	pauseButton = (CheckBox*)parent->GetChild("PauseB");
	musicButton = (CheckBox*)parent->GetChild("MusicB");
	musicPlayer = (MusicPlayer*)parent->GetChild("MusicP");
	
	if (musicButton && GameState::Instance().DoesMusicPlay()) {
		musicButton->SetState(true);
		musicPlayer->Play();
	}

	if (player) {
		maxHP = player->GetMaxHP();
		prevHP = 0;
	}

	SubscribeControlChanged(pauseButton.Get(), SPH_BIND(CamControl::OnPauseButtonPressed));
	SubscribeControlChanged(musicButton.Get(), SPH_BIND(CamControl::OnMusicButtonPressed));
	SubscribeControlChanged((Control*)pausePanel->GetChild("ResumeB"), SPH_BIND(CamControl::OnResume));
	SubscribeControlChanged((Control*)pausePanel->GetChild("RetryB"), SPH_BIND(CamControl::OnRetry));
	SubscribeControlChanged((Control*)pausePanel->GetChild("MenuB"), SPH_BIND(CamControl::OnMenu));
	SubscribeControlChanged((Control*)finishPanel->GetChild("RetryB_1"), SPH_BIND(CamControl::OnRetry));
	SubscribeControlChanged((Control*)finishPanel->GetChild("MenuB_1"), SPH_BIND(CamControl::OnMenu));
	SubscribeControlChanged((Control*)goPanel->GetChild("RetryB_2"), SPH_BIND(CamControl::OnRetry));
	SubscribeControlChanged((Control*)goPanel->GetChild("MenuB_2"), SPH_BIND(CamControl::OnMenu));

	if (GameState::Instance().GetCurrLevel() != 3) {
		SubscribeControlChanged((Control*)finishPanel->GetChild("NextB"), SPH_BIND(CamControl::OnNext));
	}
}


bool CamControl::IsCompatibleWithParent(GameObject* parent)
{
	if (parent == nullptr) {
		return false;
	}

	if (!parent->Is(Type::Camera)) {
		return false;
	}

	return true;
}

