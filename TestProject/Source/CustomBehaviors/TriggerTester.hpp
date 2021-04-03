#pragma once

#include "LocalListenerBehavior.hpp"


class SPH_EXPORT TriggerTester : public Sharpheus::LocalListenerBehavior
{
public:
	TriggerTester(Sharpheus::Behavior* other);
	TriggerTester(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~TriggerTester();

	SPH_DECL_BEHAVIOR(TriggerTester, 3);

protected:
	void Tick(float deltaTime) override;
	void DoSubscriptions();

	void OnTriggerEnter(const Sharpheus::OnEnterEvent& e);
	void OnTriggerExit(const Sharpheus::OnExitEvent& e);
};

