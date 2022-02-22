#pragma once

#include "CustomMacros.h"
#include "Behavior.hpp"
#include "LocalListeners/ControlListener.hpp"


class SPH_CUSTOM_EXPORT Controller : public Sharpheus::Behavior, public Sharpheus::ControlListener
{
public:
	Controller(Sharpheus::Behavior* other);
	Controller(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~Controller();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(Controller, 5);

protected:
	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnButtonPress(const Sharpheus::ControlChangedEvent& e);
};

