#pragma once

#ifdef SPH_BUILD_ENGINE
	#define SPH_EXPORT __declspec(dllexport)
#else
	#define SPH_EXPORT __declspec(dllimport)
#endif

#ifdef SPH_DEBUG
	#define SPH_BREAK() __debugbreak()
#else
	#define SPH_BREAK() 
#endif


// Binds
#define SPH_BIND_0(x) std::bind(&x)
#define SPH_BIND_1(x) std::bind(&x, std::placeholders::_1)
#define SPH_BIND_2(x) std::bind(&x, std::placeholders::_1, std::placeholders::_2)
#define SPH_BIND_3(x) std::bind(&x, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

#define SPH_BIND_THIS_0(x) std::bind(&x, this)
#define SPH_BIND_THIS_1(x) std::bind(&x, this, std::placeholders::_1)
#define SPH_BIND_THIS_2(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2)
#define SPH_BIND_THIS_3(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

#define SPH_BIND_WITH_0(x, a) std::bind(&x, a)
#define SPH_BIND_WITH_1(x, a) std::bind(&x, a, std::placeholders::_1)
#define SPH_BIND_WITH_2(x, a) std::bind(&x, a, std::placeholders::_1, std::placeholders::_2)
#define SPH_BIND_WITH_3(x, a) std::bind(&x, a, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

#define SPH_BIND_THIS_WITH_0(x, a) std::bind(&x, this, a)
#define SPH_BIND_THIS_WITH_1(x, a) std::bind(&x, this, a, std::placeholders::_1)
#define SPH_BIND_THIS_WITH_2(x, a) std::bind(&x, this, a, std::placeholders::_1, std::placeholders::_2)
#define SPH_BIND_THIS_WITH_3(x, a) std::bind(&x, this, a, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

#define SPH_BIND(x) SPH_BIND_THIS_1(x)
#define SPH_BIND_GETTER(x) SPH_BIND_1(x)
#define SPH_BIND_SETTER(x) SPH_BIND_2(x)
#define SPH_BIND_STATIC(x) SPH_BIND_1(x)


// Logs
#ifndef SPH_FINAL
	#define SPH_DO_LOGGING
#endif

#ifdef SPH_DO_LOGGING
	#define SPH_LOG(...) ::Sharpheus::Logger::GetEngineLogger()->trace(__VA_ARGS__)
	#define SPH_INFO(...) ::Sharpheus::Logger::GetEngineLogger()->info( __VA_ARGS__)
	#define SPH_WARN(...) ::Sharpheus::Logger::GetEngineLogger()->warn(__VA_ARGS__)
	#define SPH_ERROR(...) ::Sharpheus::Logger::LogEngineError(__VA_ARGS__)
	#define SPH_FATAL(...) ::Sharpheus::Logger::LogEngineFatal(__VA_ARGS__)

	#define SPH_ASSERT_0(cond) { if (!(cond)) SPH_ERROR(#cond); }
	#define SPH_ASSERT(cond, ...) { if (!(cond)) SPH_ERROR(std::string(#cond) + " " + __VA_ARGS__); }
	#define SPH_VERIFY_0(cond) (!(cond) ? [&]() -> bool {SPH_ERROR(#cond); return false; }() : true )
	#define SPH_VERIFY(cond, ...) (!(cond) ? ([&]() -> bool {SPH_ERROR(std::string(#cond) + " " + __VA_ARGS__); return false; }()) : true )
#else
	#define SPH_LOG(...)
	#define SPH_INFO(...)
	#define SPH_WARN(...)
	#define SPH_ERROR(...)
	#define SPH_FATAL(...)

	#define SPH_ASSERT_0(cond)
	#define SPH_ASSERT(cond, ...)
	#define SPH_VERIFY_0(cond) cond
	#define SPH_VERIFY(cond, ...) cond
#endif
