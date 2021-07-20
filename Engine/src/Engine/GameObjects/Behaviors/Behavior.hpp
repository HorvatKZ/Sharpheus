#pragma once

#include "../GameObject.hpp"

#define SPH_DECL_BEHAVIOR(Class, subType) \
	static const ::Sharpheus::ClassInfo classInfo; \
	static const uint32 classVersion; \
	virtual inline uint32 GetSubType() override { return subType; } \
	virtual inline ::Sharpheus::ClassInfoPtr GetBehaviorClassInfo() override { return &Class::classInfo; } \
	virtual void Init() override; \
	static bool IsCompatibleWithParent(::Sharpheus::GameObject* parent);

#define SPH_CHECK_CLASSVERSION(fl, version) \
	uint32 oldVersion; \
	fl.Read(oldVersion); \
	if (oldVersion != version) \
		return fl.ReadLine();

#define SPH_SUBCHECKTYPE(variable, subType) \
	if (((Behavior*)variable)->GetSubType() != subType) { \
		SPH_ERROR("\"{0}\" tries to copy from different type \"{1}\"", name, variable->GetName()); \
		return; \
	}

#define SPH_COPY_HEADER(subType, parent) \
	SPH_CHECKTYPE(other, Behavior); \
	SPH_SUBCHECKTYPE(other, subType); \
	parent::CopyContent(other);


namespace Sharpheus {

	class SPH_EXPORT Behavior : public GameObject
	{
	public:
		Behavior(Behavior* other);
		Behavior(GameObject* parent, const std::string& name) : GameObject(parent, name) {}
		virtual ~Behavior() = default;
		void CopyFrom(GameObject* other) override;

		void SetLevel(class Level* level) override;

		virtual void Init() = 0;

		virtual inline uint32 GetSubType() = 0;

		virtual inline ClassInfoPtr GetBehaviorClassInfo() = 0;

		SPH_DECL_GAMEOBJECT(Behavior);

	protected:
		virtual bool Save(FileSaver& file);

		virtual void CopyContent(GameObject* other);
	};
}