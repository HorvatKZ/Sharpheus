#pragma once

#ifndef SPH_EXPORTED
	#ifdef SPH_BUILD_CUSTOM
		#define SPH_CUSTOM_EXPORT __declspec(dllexport)
	#else
		#define SPH_CUSTOM_EXPORT __declspec(dllimport)
	#endif
#else
	#define SPH_CUSTOM_EXPORT
#endif


#define SPH_START_PRESENTING(obj) { switch (((Behavior*)obj)->GetSubType()) {
#define SPH_END_PRESENTING() default: SPHE_ERROR("Unexpected Behavior type during the creation of presenters"); } }
#define SPH_PRESENT(Class, subType) case subType: CreatePresenterFrom<Class>(provider, y); break;


#define SPH_CREATE_BEHAVIOR_1(Class, subType, param1) case subType: return new Class(param1);
#define SPH_CREATE_BEHAVIOR_2(Class, subType, param1, param2) case subType: return new Class(param1, param2);
#define SPH_IS_BEHAVIOR_COMP(Class, subType, param1) case subType: return Class::IsCompatibleWithParent(param1);

#define SPH_START_BEHAVIORS(subType) switch (subType) {
#define SPH_END_BEHAVIOR_CREATION(subType) } \
	SPH_ERROR("BehaviorCreator: Cannot create Behavior from subType {0}", subType); \
	return nullptr;

#define SPH_END_BEHAVIOR_COMP(subType) } \
	SPH_ERROR("BehaviorCreator: Unhandled subType {0} at IsCompatibleWithParent call", subType); \
	return nullptr;