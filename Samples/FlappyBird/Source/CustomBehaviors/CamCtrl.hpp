#pragma once

#include "CustomMacros.h"
#include "Behavior.hpp"
#include "LocalListeners/ControlListener.hpp"
#include "Engine/GameObjects/Renderables/Text.hpp"
#include "Engine/GameObjects/Renderables/Quad.hpp"


class SPH_CUSTOM_EXPORT CamCtrl : public Sharpheus::Behavior, public Sharpheus::ControlListener
{
public:
	CamCtrl(Sharpheus::Behavior* other);
	CamCtrl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~CamCtrl();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(CamCtrl, 4);

protected:
	Sharpheus::SafeGameObject bird;
	Sharpheus::SafeObject<Sharpheus::Text> scoreText;
	Sharpheus::SafeObject<Sharpheus::Quad> goPanel;

	uint32_t prevPoints = 0;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnRetry(const Sharpheus::ControlChangedEvent& e);
	void OnBack(const Sharpheus::ControlChangedEvent& e);
};

