#pragma once

#include <functional>

namespace pybind11 {
	class object;
}

namespace py = ::pybind11;

#ifdef SPH_DO_LOGGING
	#define SPH_PYEXEC(msg, ...) PythonInterface::Exec(msg, __VA_ARGS__)
#else
	#define SPH_PYEXEC(msg, ...) PythonInterface::Exec(__VA_ARGS__)
#endif

namespace Sharpheus {

	class SPH_EXPORT PythonInterface
	{
	public:
		static void Init();
		static void Clear();

		static py::object* Import(const std::string& moduleName);

#ifdef SPH_DO_LOGGING
		static void Exec(const std::string& info, const std::function<void()>& func);
#else
		static void Exec(const std::function<void()>& func);
#endif
	private:
		static bool interpreter_inited;
		static std::unordered_map<std::string, py::object*> loadedModules;
	};

}