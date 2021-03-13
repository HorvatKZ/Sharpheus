#pragma once

#include "DebugBehavior.hpp"


class SPH_EXPORT asd : public DebugBehavior
{
public:
		asd(Sharpheus::Behavior* other);
		asd(Sharpheus::GameObject* parent, const std::string& name);
		virtual ~asd();

	SPH_DECL_BEHAVIOR(asd,3);

protected:
	void Tick(float deltaTime) override;
	void Init();
};

