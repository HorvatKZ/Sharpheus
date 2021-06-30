#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ControlListener.hpp"


class SPH_EXPORT MenuControl : public Sharpheus::Behavior, public Sharpheus::ControlListener
{
public:
	MenuControl(Sharpheus::Behavior* other);
	MenuControl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~MenuControl();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(MenuControl, 3);

protected:
	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnPlay(const Sharpheus::ControlChangedEvent& e);
	void OnExit(const Sharpheus::ControlChangedEvent& e);
};

