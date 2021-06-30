#pragma once

#include "EnemyControl.hpp"


class SPH_EXPORT SpikerControl : public EnemyControl
{
public:
	SpikerControl(Sharpheus::Behavior* other);
	SpikerControl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~SpikerControl();

	inline float GetSpeed() { return speed; }
	inline float GetRange() { return range; }
	inline void SetSpeed(float speed) { this->speed = speed; }
	inline void SetRange(float range) { this->range = range; }

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(SpikerControl, 3);

protected:
	float speed = 0.f, range = 0.f;
	float currPlaceInRange = 0.f;
	bool movesToRight = true;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;
};

