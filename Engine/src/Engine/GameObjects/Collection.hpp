#pragma once

#include "GameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT Collection : public GameObject
	{
	public:
		Collection(GameObject* parent, const std::string& name);
		virtual ~Collection();
		virtual GameObject* Copy() override;

		SPH_DECL_GAMEOBJECT(Collection)

	protected:
		virtual void Tick(float deltaTime) override {}
		virtual void Render() override {}
	};

}