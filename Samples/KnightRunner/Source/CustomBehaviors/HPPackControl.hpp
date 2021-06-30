#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"
#include "PlayerController.hpp"


class SPH_EXPORT HPPackControl : public Sharpheus::Behavior, public Sharpheus::ColliderListener
{
public:
	HPPackControl(Sharpheus::Behavior* other);
	HPPackControl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~HPPackControl();

	inline uint32_t GetHeal() { return heal; }
	inline void SetHeal(uint32_t heal) { this->heal = heal; }

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(HPPackControl, 5);

protected:
	uint32_t heal;

	Sharpheus::SafeObject<PlayerController> player;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnTriggerEnter(const Sharpheus::OnEnterEvent& e);
};

