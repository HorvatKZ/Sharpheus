#pragma once

#include <functional>

namespace pybind11 {
	class object;
}

namespace Sharpheus {

	class SPH_EXPORT PythonInterface
	{
	public:
		static void Init();
		static void Clear();

		static pybind11::object* Import(const std::string& moduleName);

		static void Exec(const std::string& info, const std::function<void()>& func);
	private:
		static bool interpreter_inited;
	};

}