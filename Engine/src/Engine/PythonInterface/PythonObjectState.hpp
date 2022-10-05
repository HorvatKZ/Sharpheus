#pragma once

#include "Engine/FileUtils/FileLoader.hpp"
#include "Engine/FileUtils/FileSaver.hpp"
#include "Engine/EditorInterface/Provider.hpp"


namespace pybind11 {
	class object;
	class list;
}

namespace py = ::pybind11;

namespace Sharpheus {

	class PythonObjectState
	{
	public:
		PythonObjectState() = default;
		~PythonObjectState();
		PythonObjectState(const PythonObjectState& other);
		PythonObjectState& operator=(const PythonObjectState& other);

		void Clear();

		void FillFrom(const py::object& source, const py::list& argNames);
		void FillTo(py::object& target);

		void Save(FileSaver& fs);
		void Load(FileLoader& fl);

		std::vector<CommonProvider*> ToProviders();

	private:
		std::unordered_map<std::string, py::object*> states;
		std::string temp; // Temporary store string, when provider returns

		bool IsInternalTypeStr(const std::string& typeName);
		std::string GetTypeStr(const py::object* obj);

		void SaveObject(FileSaver& fs, const py::object* obj);

		py::object* LoadObject(FileLoader& fl);
		template <typename T, class PyType> py::object* LoadBuiltInType(FileLoader& fl);
		template <typename T> py::object* LoadInternalReference(FileLoader& fl);
		template <typename T> py::object* LoadResource(FileLoader& fl);

		CommonProvider* ProviderOf(const std::string& name, py::object* obj);
		template <class ProviderClass, typename T, class PyType> CommonProvider* ProviderOfBuiltInType(const std::string& name);
		template <class ProviderClass, typename T> CommonProvider* ProviderOfInternalReference(const std::string& name);
		template <class ProviderClass, typename T> CommonProvider* ProviderOfResource(const std::string& name);
	};

}