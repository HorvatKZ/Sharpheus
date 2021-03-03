#pragma once

#include "editor_pch.h"

namespace Sharpheus {

	class ClassRegistry
	{
	public:
		static void Init();
		static void Clear();

		static inline ClassInfo* GetClassInfo(GameObject::Type type) { return classInfos[type]; }
		static inline ClassInfo* GetClassInfo(GameObject* obj) {
			GameObject::Type type = obj->GetType();
			return type == GameObject::Type::Behavior ? ((Behavior*)obj)->GetBehaviorClassInfo() : classInfos[type];
		}

		static std::unordered_map<GameObject::Type, const std::string&> GetAllNames();
		static std::unordered_map<GameObject::Type, const std::string&> GetAllIconPaths();

	private:
		static std::unordered_map<GameObject::Type, ClassInfo*> classInfos;
	};

}