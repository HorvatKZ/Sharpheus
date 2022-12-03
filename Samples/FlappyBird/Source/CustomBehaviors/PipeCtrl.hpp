#pragma once

#include "CustomMacros.h"
#include "Behavior.hpp"


class SPH_CUSTOM_EXPORT PipeCtrl : public Sharpheus::Behavior
{
public:
	PipeCtrl(Sharpheus::Behavior* other);
	PipeCtrl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~PipeCtrl();

	inline float GetSpawnDist() { return spawnDist; }
	inline float GetXDistance() { return xDistance; }
	inline float GetXDistribution() { return xDistribution; }
	inline float GetYDistribution() { return yDistribution; }
	inline float GetYDiff() { return yDiff; }

	inline void SetSpawnDist(float spawnDist) { this->spawnDist = spawnDist; }
	inline void SetXDistance(float xDistance) { this->xDistance = xDistance; }
	inline void SetXDistribution(float xDistribution) { this->xDistribution = xDistribution; }
	inline void SetYDistribution(float yDistribution) { this->yDistribution = yDistribution; }
	inline void SetYDiff(float yDiff) { this->yDiff = yDiff; }

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(PipeCtrl, 5);

protected:
	float spawnDist = 2000.f, xDistance = 500.f, xDistribution = 100.f, yDistribution = 300.f, yDiff = 200.f;

	Sharpheus::SafeGameObject bird;
	std::list<Sharpheus::SafeGameObject> pipes;
	std::list<Sharpheus::SafeGameObject>::iterator it;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void Fill();
	void Spawn(const Sharpheus::Point& center);
	float GetRand(float rangeBottom, float rangeTop);
};

