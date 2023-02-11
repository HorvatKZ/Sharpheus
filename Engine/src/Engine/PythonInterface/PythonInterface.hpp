#pragma once

#include <functional>

namespace pybind11 {
	class object;
}

namespace py = ::pybind11;


namespace Sharpheus {

	class SPH_EXPORT PythonInterface
	{
	public:
		static void Init();
		static void Clear();

		static void ClearModules();
		static void ReloadModules();

		static py::object* Import(const std::string& moduleName);

		static bool Exec(const std::string& info, const std::function<void()>& func, bool onlyLogOnError = false);

		static bool Precompile(const std::string& pyFile, const std::string& pycResult);

		static const std::string moduleName;
	private:
		static bool interpreter_inited;
		static std::unordered_map<std::string, py::object*> loadedModules;
	};

}