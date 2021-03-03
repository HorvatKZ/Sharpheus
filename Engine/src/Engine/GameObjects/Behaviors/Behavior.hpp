#pragma once

#include "../GameObject.hpp"

#define SPH_DECL_BEHAVIOR(Class, subType) \
	static ::Sharpheus::ClassInfo classInfo; \
	virtual inline uint32_t GetSubType() override { return subType; } \
	virtual inline ::Sharpheus::ClassInfo* GetBehaviorClassInfo() override { return &Class::classInfo; } \
	static bool IsCompatibleWithParent(::Sharpheus::GameObject* parent);

#define SPH_SUBCHECKTYPE(variable, subType) \
	if (((Behavior*)variable)->GetSubType() != subType) { \
		SPH_ERROR("\"{0}\" tries to copy from different type \"{1}\"", name, variable->GetName()); \
		return; \
	}

#define SPH_COPY_HEADER(subType) \
	SPH_CHECKTYPE(other, Behavior); \
	SPH_SUBCHECKTYPE(other, subType); \
	Behavior::CopyFrom(other);


namespace Sharpheus {

	class SPH_EXPORT Behavior : public GameObject
	{
	public:
		Behavior(Behavior* other) : GameObject(std::move(*other)) { delete other; other = nullptr; }
		Behavior(GameObject* parent, const std::string& name) : GameObject(parent, name) {}
		virtual ~Behavior() = default;

		virtual inline uint32_t GetSubType() = 0;

		virtual inline ClassInfo* GetBehaviorClassInfo() = 0;

		SPH_DECL_GAMEOBJECT(Behavior);

	protected:
		virtual void Render() override {}

		virtual bool Save(FileSaver& file);
	};
}