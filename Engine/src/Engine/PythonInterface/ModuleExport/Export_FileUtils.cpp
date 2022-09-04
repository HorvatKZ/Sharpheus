#include "pch.h"
#include "PydExport.hpp"
#include "Engine/FileUtils/FileLoader.hpp"
#include "Engine/FileUtils/FileSaver.hpp"
#include "Engine/FileUtils/OSPaths.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	void Export_FileLoader(py::module_& handle)
	{
		py::class_<FileLoader>(handle, "FileLoader")
			.def(py::init<const std::string&>(), "path"_a)
			.def_property_readonly("status", &FileLoader::GetStatus)

			.def("read", py::overload_cast<uint8&>(&FileLoader::Read), "data"_a)
			.def("read", py::overload_cast<uint32&>(&FileLoader::Read), "data"_a)
			.def("read", py::overload_cast<int32&>(&FileLoader::Read), "data"_a)
			.def("read", py::overload_cast<size_t&>(&FileLoader::Read), "data"_a)
			.def("read", py::overload_cast<bool&>(&FileLoader::Read), "data"_a)
			.def("read", py::overload_cast<float&>(&FileLoader::Read), "data"_a)
			.def("read", py::overload_cast<std::string&>(&FileLoader::Read), "data"_a)
			.def("read", py::overload_cast<Point&>(&FileLoader::Read), "data"_a)
			.def("read", py::overload_cast<Color&>(&FileLoader::Read), "data"_a)
			.def("read", py::overload_cast<Transform&>(&FileLoader::Read), "data"_a)

			.def("read_line", &FileLoader::ReadLine)
			.def("try_reading_end", &FileLoader::TryReadingEnd);
	}


	void Export_FileSaver(py::module_& handle)
	{
		py::class_<FileSaver>(handle, "FileSaver")
			.def(py::init<const std::string&>(), "path"_a)
			.def_property_readonly("status", &FileSaver::GetStatus)

			.def("read", py::overload_cast<uint8>(&FileSaver::Write), "data"_a)
			.def("read", py::overload_cast<uint32>(&FileSaver::Write), "data"_a)
			.def("read", py::overload_cast<int32>(&FileSaver::Write), "data"_a)
			.def("read", py::overload_cast<size_t>(&FileSaver::Write), "data"_a)
			.def("read", py::overload_cast<bool>(&FileSaver::Write), "data"_a)
			.def("read", py::overload_cast<float>(&FileSaver::Write), "data"_a)
			.def("read", py::overload_cast<const std::string&>(&FileSaver::Write), "data"_a)
			.def("read", py::overload_cast<const Point&>(&FileSaver::Write), "data"_a)
			.def("read", py::overload_cast<const Color&>(&FileSaver::Write), "data"_a)
			.def("read", py::overload_cast<const Transform&>(&FileSaver::Write), "data"_a)

			.def("write_end", &FileSaver::WriteEnd);
	}


	void Export_OSPaths_Module(py::module_& handle)
	{
		py::module_ m = handle.def_submodule("OSPaths");
		m.doc() = "Module for querying OS related paths for Sharpheus";

		py::enum_<OSPaths::Folder>(m, "Folder")
			.value("APPDATA_ROAMING", OSPaths::Folder::APPDATA_ROAMING)
			.value("EXEC_FOLDER", OSPaths::Folder::EXEC_FOLDER);

		m.def("get", &OSPaths::Get, "folder"_a);
	}


	void Export_FileUtils(py::module_& handle)
	{
		Export_FileLoader(handle);
		Export_FileSaver(handle);
		Export_OSPaths_Module(handle);
	}

}