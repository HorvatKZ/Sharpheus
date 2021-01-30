#pragma once

#include "pch.h"
#include "GameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT Collection : public GameObject
	{
	public:
		Collection(const std::string& name, const Transform& trafo = Transform()) : GameObject(name, trafo) {}
		virtual ~Collection() = default;

	protected:
		virtual void TickThis(float deltatime) override {}
		virtual void RenderThis() override {}
	};

}