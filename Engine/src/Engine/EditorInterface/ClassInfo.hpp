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

		inline const std::string& GetName() { return name; }
		inline const std::string& GetIconPath() { return iconPath; }
		inline const std::vector<CommonProvider*>& GetProviders() { return providers; }

	private:
		const std::string name, iconPath;
		const std::vector<CommonProvider*> providers;
	};
}