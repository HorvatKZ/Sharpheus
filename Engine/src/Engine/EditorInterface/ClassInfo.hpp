#pragma once

#include "Provider.hpp"


namespace Sharpheus {

	class SPH_EXPORT ClassInfo
	{
	public:
		ClassInfo(const std::string& name, const std::string& iconPath, const std::vector<CommonProvider*>& providers)
			: name(name), iconPath(iconPath), providers(providers) {}
		virtual ~ClassInfo() { Free(); }

		void Free() {
			for (CommonProvider* provider : providers) {
				delete provider;
			}
		}

		inline const std::string& GetName() const { return name; }
		inline const std::string& GetIconPath() const { return iconPath; }
		inline const std::vector<CommonProvider*>& GetProviders() const { return providers; }

	private:
		const std::string name, iconPath;
		const std::vector<CommonProvider*> providers;
	};

	using ClassInfoPtr = const ClassInfo*;

}


#define SPH_START_CLASSINFO(Class, Icon) const ::Sharpheus::ClassInfo Class::classInfo(#Class, Icon, { \

#define SPH_START_SPEC_CLASSINFO(Class, DisplayName, Icon) const ::Sharpheus::ClassInfo Class::classInfo(DisplayName, Icon, { \

#define SPH_END_CLASSINFO });
