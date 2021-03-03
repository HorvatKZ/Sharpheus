#pragma once

#ifdef SPH_BUILD_ENGINE
	#define SPH_EXPORT __declspec(dllexport)
#else
	#define SPH_EXPORT __declspec(dllimport)
#endif


#define SPH_BREAK __debugbreak

// Binds
#define SPH_BIND_0(x) std::bind(&x)
#define SPH_BIND_1(x) std::bind(&x, std::placeholders::_1)
#define SPH_BIND_2(x) std::bind(&x, std::placeholders::_1, std::placeholders::_2)
#define SPH_BIND_3(x) std::bind(&x, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define SPH_BIND_THIS_0(x) std::bind(&x, this)
#define SPH_BIND_THIS_1(x) std::bind(&x, this, std::placeholders::_1)
#define SPH_BIND_THIS_2(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2)
#define SPH_BIND_THIS_3(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

#define SPH_BIND(x) SPH_BIND_THIS_1(x)
#define SPH_BIND_GETTER(x) SPH_BIND_1(x)
#define SPH_BIND_SETTER(x) SPH_BIND_2(x)
#define SPH_BIND_STATIC(x) SPH_BIND_1(x)


// Logs
#define SPH_LOG(...) ::Sharpheus::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define SPH_INFO(...) ::Sharpheus::Logger::GetEngineLogger()->info( __VA_ARGS__)
#define SPH_WARN(...) ::Sharpheus::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define SPH_ERROR(...) ::Sharpheus::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define SPH_FATAL(...) ::Sharpheus::Logger::GetEngineLogger()->critical(__VA_ARGS__)
#define SPH_ASSERT(cond, ...) { if (!(cond)) { SPH_ERROR(__VA_ARGS__); SPH_BREAK(); } }