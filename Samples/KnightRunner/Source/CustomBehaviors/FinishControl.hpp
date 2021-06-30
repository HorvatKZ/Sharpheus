#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"


class SPH_EXPORT FinishControl : public Sharpheus::Behavior, public Sharpheus::ColliderListener
{
public:
	FinishControl(Sharpheus::Behavior* other);
	FinishControl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~FinishControl();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(FinishControl, 6);

protected:
	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnTrigger(const Sharpheus::OnEnterEvent& e);
};

