#pragma once

#include <functional>

namespace Sharpheus {

	class SPH_EXPORT PythonInterface
	{
	public:
		static void Init();
		static void Clear();

		static void Exec(const std::function<void()>& func);
	private:
		static bool interpreter_inited;
	};

}