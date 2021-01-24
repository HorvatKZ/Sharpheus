#pragma once

#ifdef SPH_BUILD_ENGINE
	#define SPH_EXPORT __declspec(dllexport)
#else
	#define SPH_EXPORT __declspec(dllimport)
#endif


#define SPH_BIND(x) std::bind(&x, this, std::placeholders::_1)
#define SPH_BIND_STATIC(x) std::bind(&x, std::placeholders::_1)


// Logs
#define SPH_LOG(...) ::Sharpheus::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define SPH_INFO(...) ::Sharpheus::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define SPH_WARN(...) ::Sharpheus::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define SPH_ERROR(...) ::Sharpheus::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define SPH_FATAL(...) ::Sharpheus::Logger::GetEngineLogger()->critical(__VA_ARGS__)
#define SPH_ASSERT(cond, ...) { if (!(cond)) SPH_ERROR(__VA_ARGS__); }