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

		template <typename... Args>
		static inline void LogEngineError(Args&&... args) { Logger::GetEngineLogger()->error(std::forward<Args>(args)...); SPH_BREAK(); }

		template <typename... Args>
		static inline void LogEngineFatal(Args&&... args) { Logger::GetEngineLogger()->error(std::forward<Args>(args)...); SPH_BREAK(); }

	private:
		static std::shared_ptr<spdlog::logger> engineLogger;
		static std::shared_ptr<spdlog::logger> editorLogger;
	};

}