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

		static py::object* Import(const std::string& moduleName);

		static void Exec(const std::string& info, const std::function<void()>& func);

		static const std::string moduleName;
	private:
		static bool interpreter_inited;
		static std::unordered_map<std::string, py::object*> loadedModules;
	};

}