#pragma once

#include "CustomMacros.h"
#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"


class SPH_CUSTOM_EXPORT BirdCtrl : public Sharpheus::Behavior, public Sharpheus::ColliderListener
{
public:
	BirdCtrl(Sharpheus::Behavior* other);
	BirdCtrl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~BirdCtrl();

	inline float GetJumpForce() { return jumpForce; }
	inline void SetJumpForce(float jumpForce) { this->jumpForce = jumpForce; }

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(BirdCtrl, 3);

protected:
	float jumpForce = 0.f;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnClick(const Sharpheus::MousePressedEvent& e);
	void OnCollision(const Sharpheus::CollisionEvent& e);
};

