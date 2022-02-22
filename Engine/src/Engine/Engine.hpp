#pragma once


namespace Sharpheus {

	class SPH_EXPORT Engine
	{
	public:
		static void Init(class BehaviorCreatorBase* bc);
		static void Clear();
	};

}