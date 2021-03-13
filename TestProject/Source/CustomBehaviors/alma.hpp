#pragma once

#include "Behavior.hpp"


class SPH_EXPORT alma : public Sharpheus::Behavior
{
public:
		alma(Sharpheus::Behavior* other);
		alma(Sharpheus::GameObject* parent, const std::string& name);
		virtual ~alma();

	SPH_DECL_BEHAVIOR(alma,3);

protected:
	void Tick(float deltaTime) override;
	void Init();
};

