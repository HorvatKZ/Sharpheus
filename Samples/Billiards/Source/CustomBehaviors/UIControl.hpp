#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ControlListener.hpp"
#include "Engine/GameObjects/Renderables/Sprite.hpp"
#include "Engine/GameObjects/Renderables/Text.hpp"
#include "Engine/GameObjects/Renderables/Quad.hpp"
#include "Engine/GameObjects/Controls/Button.hpp"


class SPH_EXPORT UIControl : public Sharpheus::Behavior, public Sharpheus::ControlListener
{
public:
	UIControl(Sharpheus::Behavior* other);
	UIControl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~UIControl();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(UIControl, 2);

protected:
	Sharpheus::SafeObject<Sharpheus::Sprite> p11, p12, p21, p22;
	Sharpheus::SafeObject<Sharpheus::Text> p1Points, p2Points, go, winnerName;
	Sharpheus::SafeObject<Sharpheus::Quad> winnerPanel;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnBackToMenu(const Sharpheus::ControlChangedEvent& e);
};

