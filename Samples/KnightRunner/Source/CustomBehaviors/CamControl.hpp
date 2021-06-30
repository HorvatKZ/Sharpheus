#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ControlListener.hpp"
#include "PlayerController.hpp"
#include "Engine/GameObjects/Renderables/Quad.hpp"
#include "Engine/GameObjects/Renderables/Text.hpp"
#include "Engine/GameObjects/Controls/CheckBox.hpp"
#include "Engine/GameObjects/Sounds/MusicPlayer.hpp"


class SPH_EXPORT CamControl : public Sharpheus::Behavior, public Sharpheus::ControlListener
{
public:
	CamControl(Sharpheus::Behavior* other);
	CamControl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~CamControl();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(CamControl, 4);

protected:
	Sharpheus::SafeObject<PlayerController> player;
	Sharpheus::SafeObject<Sharpheus::Quad> hpBar;
	Sharpheus::SafeObject<Sharpheus::Text> hpText;
	Sharpheus::SafeObject<Sharpheus::Quad> pausePanel, finishPanel, goPanel;
	Sharpheus::SafeObject<Sharpheus::CheckBox> pauseButton, musicButton;
	Sharpheus::SafeObject<Sharpheus::MusicPlayer> musicPlayer;

	uint32_t maxHP, prevHP;
	bool canUseButtons = true;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnPauseButtonPressed(const Sharpheus::ControlChangedEvent& e);
	void OnMusicButtonPressed(const Sharpheus::ControlChangedEvent& e);
	void OnResume(const Sharpheus::ControlChangedEvent& e);
	void OnRetry(const Sharpheus::ControlChangedEvent& e);
	void OnMenu(const Sharpheus::ControlChangedEvent& e);
	void OnNext(const Sharpheus::ControlChangedEvent& e);

	void GameOver();
	void Finish();
};

