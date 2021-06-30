#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ControlListener.hpp"


class SPH_EXPORT MenuCtrl : public Sharpheus::Behavior, public Sharpheus::ControlListener
{
public:
	MenuCtrl(Sharpheus::Behavior* other);
	MenuCtrl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~MenuCtrl();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(MenuCtrl, 4);

protected:
	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnPlay(const Sharpheus::ControlChangedEvent& e);
	void OnExit(const Sharpheus::ControlChangedEvent& e);
};

