#pragma once

#include "GameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT Collection : public GameObject
	{
	public:
		Collection(GameObject* parent, const std::string& name);
		virtual ~Collection();

		SPH_DECL_GAMEOBJECT(Collection)
	};

}