#include "editor_pch.h"
#include "ClassRegistry.hpp"


namespace Sharpheus {

	std::unordered_map<GameObject::Type, ClassInfo*> ClassRegistry::classInfos;

	void ClassRegistry::Init()
	{
		classInfos[GameObject::Type::Collection]				= &Collection::classInfo;
		classInfos[GameObject::Type::Behavior]					= &Behavior::classInfo;
		classInfos[GameObject::Type::Camera]					= &Camera::classInfo;
		classInfos[GameObject::Type::Sprite]					= &Sprite::classInfo;
		classInfos[GameObject::Type::PhysicsObject]				= &PhysicsObject::classInfo;
		classInfos[GameObject::Type::BoxCollider]				= &BoxCollider::classInfo;
	}

	void ClassRegistry::Clear()
	{
	}

	std::unordered_map<GameObject::Type, const std::string&> ClassRegistry::GetAllNames()
	{
		std::unordered_map<GameObject::Type, const std::string&> result;

		for (auto it = classInfos.begin(); it != classInfos.end(); ++it) {
			result.insert({ (*it).first, (*it).second->GetName() });
		}

		return result;
	}

	std::unordered_map<GameObject::Type, const std::string&> ClassRegistry::GetAllIconPaths()
	{
		std::unordered_map<GameObject::Type, const std::string&> result;
		
		for (auto it = classInfos.begin(); it != classInfos.end(); ++it) {
			result.insert({ (*it).first, (*it).second->GetIconPath() });
		}

		return result;
	}
}