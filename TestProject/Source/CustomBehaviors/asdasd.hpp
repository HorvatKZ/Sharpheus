#pragma once

#include "PlayerContoller.hpp"


class SPH_EXPORT asdasd : public PlayerContoller
{
public:
		asdasd(Sharpheus::Behavior* other);
		asdasd(Sharpheus::GameObject* parent, const std::string& name);
		virtual ~asdasd();

	SPH_DECL_BEHAVIOR(asdasd,3);

protected:
	void Tick(float deltaTime) override;
	void Init();
};

