#pragma once

#include "CustomMacros.h"
#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"


class SPH_CUSTOM_EXPORT TriggerTester : public Sharpheus::Behavior, public Sharpheus::ColliderListener
{
public:
	TriggerTester(Sharpheus::Behavior* other);
	TriggerTester(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~TriggerTester();

	SPH_DECL_BEHAVIOR(TriggerTester, 5);

protected:
	void Tick(float deltaTime) override;

	void OnTriggerEnter(const Sharpheus::OnEnterEvent& e);
	void OnTriggerExit(const Sharpheus::OnExitEvent& e);
};

