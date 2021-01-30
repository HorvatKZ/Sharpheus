#pragma once

#include <spdlog/spdlog.h>


namespace Sharpheus {

	class SPH_EXPORT Logger
	{
	public:
		static void Init();
		static void Clear();

		static inline std::shared_ptr<spdlog::logger>& GetEngineLogger() { return engineLogger; }
		static inline std::shared_ptr<spdlog::logger>& GetEditorLogger() { return editorLogger; }

	private:
		static std::shared_ptr<spdlog::logger> engineLogger;
		static std::shared_ptr<spdlog::logger> editorLogger;
	};

}