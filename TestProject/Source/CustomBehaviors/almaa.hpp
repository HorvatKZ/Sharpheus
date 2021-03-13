#pragma once


#include "Behavior.hpp"




class SPH_EXPORT almaa : public Behavior
{
public:
		almaa(Sharpheus::Behavior* other);
		almaa(Sharpheus::GameObject* parent, const std::string& name);
		virtual ~almaa();


	SPH_DECL_BEHAVIOR(almaa,3);


protected:
		void Tick(float deltaTime) override
		void Init()
};


