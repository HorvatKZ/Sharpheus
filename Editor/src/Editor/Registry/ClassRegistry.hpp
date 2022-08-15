#pragma once

#include "editor_pch.h"

namespace Sharpheus {

	class ClassRegistry
	{
	public:
		static void Init();
		static void Clear();

		static inline ClassInfoPtr GetClassInfo(GameObject::Type type) { return classInfos[type]; }
		static inline ClassInfoPtr GetClassInfo(GameObject* obj) {
			GameObject::Type type = obj->GetType();
			return type == GameObject::Type::CppBehavior ? ((Behavior*)obj)->GetBehaviorClassInfo() : classInfos[type];
		}

		static std::unordered_map<GameObject::Type, const std::string&> GetAllNames();
		static std::unordered_map<GameObject::Type, const std::string&> GetAllIconPaths();

	private:
		static std::unordered_map<GameObject::Type, ClassInfoPtr> classInfos;
	};

}