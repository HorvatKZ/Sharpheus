#pragma once

#include "EnemyControl.hpp"


class SPH_EXPORT KeeseControl : public EnemyControl
{
public:
	KeeseControl(Sharpheus::Behavior* other);
	KeeseControl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~KeeseControl();

	inline float GetSpeed() { return speed; }
	inline float GetSense() { return sense; }
	inline void SetSpeed(float speed) { this->speed = speed; }
	inline void SetSense(float sense) { this->sense = sense; }

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(KeeseControl, 8);

protected:
	float speed = 0.f, sense = 0.f;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;
};

