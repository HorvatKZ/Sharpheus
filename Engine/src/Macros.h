#pragma once

#ifdef SPH_BUILD_ENGINE
	#define SPH_EXPORT __declspec(dllexport)
#else
	#define SPH_EXPORT __declspec(dllimport)
#endif


#define SPH_BIND(x) std::bind(&x, this)