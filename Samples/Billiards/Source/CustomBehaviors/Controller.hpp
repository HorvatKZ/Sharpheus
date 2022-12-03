#pragma once

#include "CustomMacros.h"
#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"
#include "Engine/GameObjects/Colliders/PhysicsObject.hpp"
#include "Engine/GameObjects/Renderables/Sprite.hpp"


class SPH_CUSTOM_EXPORT Controller : public Sharpheus::Behavior, public Sharpheus::ColliderListener
{
public:
	Controller(Sharpheus::Behavior* other);
	Controller(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~Controller();

	inline float GetSpeedMult() { return speedMulitplier; }
	inline float GetSpeedLimit() { return speedLimit; }
	inline float GetStopSpeed() { return stopSpeed; }
	inline void SetSpeedMult(float speedMultiplier) { this->speedMulitplier = speedMultiplier; }
	inline void SetSpeedLimit(float speedLimit) { this->speedLimit = speedLimit; }
	inline void SetStopSpeed(float stopSpeed) { this->stopSpeed = stopSpeed; }

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(Controller, 3);

protected:
	float speedMulitplier = 1.f, speedLimit = 500.f, stopSpeed = 20.f;

	Sharpheus::Point ballPos, mousePos, startPos;
	bool isClicked = false, inPlace = false;

	Sharpheus::SafeObject<Sharpheus::PhysicsObject> whiteBall;
	Sharpheus::SafeObject<Sharpheus::Sprite> stick;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnMouseMove(const Sharpheus::MouseMovedEvent& e);
	void OnMousePressed(const Sharpheus::MousePressedEvent& e);
	void OnMouseReleased(const Sharpheus::MouseReleasedEvent& e);
	void OnTriggerEnter(const Sharpheus::OnEnterEvent& e);
	void OnCollision(const Sharpheus::CollisionEvent& e);
};

