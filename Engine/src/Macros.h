#pragma once

#ifdef SPH_BUILD_ENGINE
	#define SPH_EXPORT __declspec(dllexport)
#else
	#define SPH_EXPORT __declspec(dllimport)
#endif


#define SPH_BIND(x) std::bind(&x, this)


// Logs
#define SPH_LOG(...) ::Sharpheus::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define SPH_LOG_INFO(...) ::Sharpheus::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define SPH_LOG_WARN(...) ::Sharpheus::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define SPH_LOG_ERROR(...) ::Sharpheus::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define SPH_LOG_FATAL(...) ::Sharpheus::Logger::GetEngineLogger()->critical(__VA_ARGS__)
#define SPH_ASSERT(cond, ...) { if (!(cond)) SPH_LOG_ERROR(__VA_ARGS__); }