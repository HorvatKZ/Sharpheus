#include "pch.h"
#include "PydExport.hpp"
#include "Engine/Level.hpp"
#include "Engine/Project.hpp"
#include "Engine/ProjectControl.hpp"
#include "Engine/EngineVersion.hpp"


namespace Sharpheus {

	void Export_EngineVersion(py::module_& handle)
	{
		py::class_<EngineVersion>(handle, "EngineVersion")
			.def_property_readonly("vnum", &EngineVersion::GetVNum)
			.def_property_readonly("vname", &EngineVersion::GetVName)
			.def_property_readonly("is_unknown", &EngineVersion::IsUnknown)
			.def_property_readonly("is_valid", &EngineVersion::IsValid)
			.def_property_readonly("is_latest", &EngineVersion::IsLatest)
			.def_static("get_current", &EngineVersion::GetCurrent)
			.def_readonly_static("latest", &EngineVersion::latest)

			.def(py::self == py::self)
			.def(py::self != py::self)
			.def(py::self < py::self)
			.def(py::self <= py::self)
			.def(py::self > py::self)
			.def(py::self >= py::self)

			.def("is_at_least", [](const EngineVersion& v, uint32 vnum) { return v.IsAtLeast(vnum); }, "vnum"_a)
			.def("is_at_least", [](const EngineVersion& v, const std::string& vname) { return v.IsAtLeast(vname); }, "vname"_a)
			.def("is_at_least", [](const EngineVersion& v, const EngineVersion& vother) { return v.IsAtLeast(vother); }, "vother"_a)

			.def_static("get_vnum_of", &EngineVersion::GetVNumOf, "vname"_a);
	}


	void Export_Level(py::module_& handle)
	{
		py::class_<Level>(handle, "Level")
			.def_property_readonly("name", &Level::GetName)
			.def_property_readonly("pure_name", &Level::GetPureName)
			.def_property_readonly("path", &Level::GetPath)
			.def_property_readonly("full_path", &Level::GetFullPath)
			.def_property_readonly("project_file_name", &Level::GetProjectFileName)
			.def_property_readonly("layer_num", &Level::GetLayerNum)
			.def_property_readonly("layer_names", &Level::GetLayerNames, py::return_value_policy::reference)
			.def_property_readonly("root", &Level::GetRoot, py::return_value_policy::reference)

			.def("create", static_cast<GameObject* (Level::*)(GameObject::Type, GameObject*, const std::string&)>(&Level::Create), "type"_a, "parent"_a, "new_name"_a, py::return_value_policy::reference)
			.def("create", static_cast<GameObject * (Level::*)(GameObject*, GameObject*, const std::string&)>(&Level::Create), "other"_a, "parent"_a, "new_name"_a, py::return_value_policy::reference)
			.def("attach", &Level::Attach, "obj"_a)
			.def("delete", &Level::Delete, "obj"_a, "force_now"_a = false)
			.def("move", &Level::Move, "obj"_a, "new_parent"_a)
			.def("move", &Level::Move, "obj"_a, "new_parent"_a)

			.def("get_gameobject", &Level::GetGameObject, "name"_a, py::return_value_policy::reference)

			.def("create_layer", py::overload_cast<const std::string&>(&Level::CreateLayer), "name"_a)
			.def("create_layer", py::overload_cast<const std::string&, uint32>(&Level::CreateLayer), "name"_a, "ind"_a)
			.def("rename_layer", py::overload_cast<uint32, const std::string&>(&Level::RenameLayer), "ind"_a, "new_name"_a)
			.def("rename_layer", py::overload_cast<const std::string&, const std::string&>(&Level::RenameLayer), "old_name"_a, "new_name"_a)
			.def("remove_layer", py::overload_cast<uint32, uint32>(&Level::RemoveLayer), "ind"_a, "move2layer"_a)
			.def("remove_layer", py::overload_cast<const std::string&, const std::string&>(&Level::RemoveLayer), "name"_a, "move2layer"_a)
			.def("swap_layer", py::overload_cast<uint32, uint32>(&Level::SwapLayers), "first_layer"_a, "second_layer"_a)
			.def("swap_layer", py::overload_cast<const std::string&, const std::string&>(&Level::SwapLayers), "first_layer"_a, "second_layer"_a)
			.def("is_layer_visible", py::overload_cast<uint32>(&Level::IsLayerVisible), "ind"_a)
			.def("is_layer_visible", py::overload_cast<const std::string&>(&Level::IsLayerVisible), "name"_a)
			.def("set_layer_visible", py::overload_cast<uint32, bool>(&Level::SetLayerVisible), "ind"_a, "visibility"_a)
			.def("set_layer_visible", py::overload_cast<const std::string&, bool>(&Level::SetLayerVisible), "name"_a, "visibility"_a)
			.def_static("is_deletable_layer", &Level::IsDeletableLayer, "layer"_a)

			.def("save_as_scene", &Level::SaveAsScene, "obj"_a, "path"_a)
			.def("attach_scene_to", &Level::AttachSceneTo, "obj"_a, "path"_a);
	}


	void Export_ProjectControl(py::module_& handle)
	{
		py::module_ m = handle.def_submodule("ProjectControl");
		m.doc() = "Project Control module for Sharpheus";

		m.def("load_level", &ProjectControl::LoadLevel, "path"_a);
		m.def("exit", &ProjectControl::Exit);
		m.def("pause", &ProjectControl::Pause);
		m.def("resume", &ProjectControl::Resume);
	}


	void Export_EngineCore(py::module_& handle)
	{
		Export_EngineVersion(handle);
		Export_Level(handle);
		Export_ProjectControl(handle);
	}

}