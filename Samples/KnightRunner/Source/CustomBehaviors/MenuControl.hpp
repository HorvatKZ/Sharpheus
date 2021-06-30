#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ControlListener.hpp"
#include "Engine/GameObjects/Controls/RadioButton.hpp"
#include "Engine/GameObjects/Controls/CheckBox.hpp"
#include "Engine/GameObjects/Sounds/MusicPlayer.hpp"


class SPH_EXPORT MenuControl : public Sharpheus::Behavior, public Sharpheus::ControlListener
{
public:
	MenuControl(Sharpheus::Behavior* other);
	MenuControl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~MenuControl();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(MenuControl, 7);

protected:
	Sharpheus::SafeGameObject mainPanel, levelSelect, settingsPanel;
	Sharpheus::SafeObject<Sharpheus::RadioButton> diffRadio;
	Sharpheus::SafeObject<Sharpheus::CheckBox> musicButton;
	Sharpheus::SafeObject<Sharpheus::MusicPlayer> musicPlayer;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnPlay(const Sharpheus::ControlChangedEvent& e);
	void OnSettings(const Sharpheus::ControlChangedEvent& e);
	void OnExit(const Sharpheus::ControlChangedEvent& e);
	void OnBack(const Sharpheus::ControlChangedEvent& e);
	void OnLevel(uint8_t level, const Sharpheus::ControlChangedEvent& e);
	void MusicButtonClicked(const Sharpheus::ControlChangedEvent& e);
};

