#include "pch.h"
#include "PythonBehavior.hpp"
#include "Engine/FileUtils/OSPaths.hpp"
#include "Engine/PythonInterface/PythonInterface.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "Engine/Level.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;


namespace Sharpheus {

	SPH_START_CLASSINFO(PythonRunnerBehavior, "behaviorpy.png")
	SPH_END_CLASSINFO


	SPH_START_CLASSINFO(PythonBehavior, "behaviorpy.png")
	SPH_END_CLASSINFO


	bool PythonBehavior::IsCompatibleWithParent(GameObject* parent)
	{
		return parent != nullptr;
	}

	void PythonBehavior::Init()
	{
	}

	ClassInfoPtr PythonBehavior::GetBehaviorClassInfo()
	{
		return &classInfo;
	}


	PythonRunnerBehavior::PythonRunnerBehavior(GameObject* parent, const std::string& name)
		: Behavior(parent, name)
	{
		objInfo = new ClassInfo("PythonRunnerBehavior", "behaviorpy.png", {
			new ScriptProvider<PythonRunnerBehavior>("Script",
				SPH_BIND_GETTER(PythonRunnerBehavior::GetModuleName),
				SPH_BIND_SETTER(PythonRunnerBehavior::SetModuleName),
				SPH_BIND_2(PythonRunnerBehavior::SetModuleNameFromScriptPath)
			)
		});
	}


	PythonRunnerBehavior::~PythonRunnerBehavior()
	{
		PythonInterface::Exec("Deleting pyObject from " + moduleName, [&] {
			if (pyBehavior != nullptr) {
				while (pyBehavior->ref_count() != 1) {
					pyBehavior->dec_ref();
				}
				delete pyBehavior;
			}
		});
		delete objInfo;
	}


	void PythonRunnerBehavior::SetModuleName(const std::string& moduleName)
	{
		if (pyBehavior != nullptr) {
			SPH_ERROR("PythonRunnerBehavior::SetModuleName call after instancing pyBehavior");
			return;
		}

		this->moduleName = moduleName;
		objectStateInited = false;
	}


	void PythonRunnerBehavior::SetModuleNameFromScriptPath(const std::string& scriptPath)
	{
		if (scriptPath.length() <= 3 || scriptPath.substr(scriptPath.length() - 3) != ".py") {
			SPH_ERROR("The given scriptPath is not a .py file");
			return;
		}
		std::string newModuleName = scriptPath.substr(0, scriptPath.length() - 3); // cutting .py
		for (uint32 i = 0; i < newModuleName.length(); ++i) {
			if (newModuleName[i] == '\\' || newModuleName[i] == '/') {
				newModuleName[i] = '.';
			}
		}
		SetModuleName(newModuleName);
	}


	bool PythonRunnerBehavior::Load(FileLoader& fl)
	{
		Behavior::Load(fl);
		std::string newModuleName;
		fl.Read(newModuleName);
		SetModuleName(newModuleName);
		InitObjectStateIfNeeded();
		state.Load(fl);
		return fl.GetStatus();
	}


	bool PythonRunnerBehavior::Save(FileSaver& fs)
	{
		Behavior::Save(fs);
		fs.Write(moduleName);
		InitObjectStateIfNeeded();
		state.Save(fs);
		return fs.GetStatus();
	}


	void PythonRunnerBehavior::CopyContent(GameObject* other)
	{
		SPH_COPY_HEADER(1, Behavior);
		PythonRunnerBehavior* trueOther = (PythonRunnerBehavior*)other;
		moduleName = trueOther->moduleName;
		objectStateInited = trueOther->objectStateInited;
		state = trueOther->state;
	}


	void PythonRunnerBehavior::InitObjectStateIfNeeded()
	{
		if (!objectStateInited) {
			if (!moduleName.empty()) {
				PythonInterface::Exec("Save PythonBehavior from " + moduleName, [&] {
					py::object* moduleObj = PythonInterface::Import(moduleName);
					if (py::hasattr(*moduleObj, "provide_list")) {
						py::list provide_list = moduleObj->attr("provide_list")();
						py::object* tempBehavior = new py::object(moduleObj->attr("create")(nullptr, "temp"));
						state.FillFrom(*tempBehavior, provide_list);
						SPH_ASSERT_0(tempBehavior->ref_count() == 1);
						delete tempBehavior;
					} else {
						state.Clear();
					}
				});
			}
			objectStateInited = true;
		}
	}


	bool PythonRunnerBehavior::IsCompatibleWithParent(GameObject* parent)
	{
		return parent != nullptr;
	}


	inline ClassInfoPtr PythonRunnerBehavior::GetBehaviorClassInfo()
	{
		InitObjectStateIfNeeded();
		if (moduleName == objInfosModuleName) {
			return objInfo;
		}

		std::vector<CommonProvider*> providers = state.ToProviders();
		providers.insert(providers.begin(),
			new ScriptProvider<PythonRunnerBehavior>("Script",
				SPH_BIND_GETTER(PythonRunnerBehavior::GetModuleName),
				SPH_BIND_SETTER(PythonRunnerBehavior::SetModuleName),
				SPH_BIND_2(PythonRunnerBehavior::SetModuleNameFromScriptPath)
			)
		);
		delete objInfo;
		objInfo = new ClassInfo("PythonRunnerBehavior", "behaviorpy.png", providers);
		objInfosModuleName = moduleName;
		return objInfo;
	}


	void PythonRunnerBehavior::Init()
	{
		PythonInterface::Exec("Creating PythonBehavior from " + moduleName, [&] {
			if (moduleName.empty()) {
				SPH_WARN("PythonRunnerBehavior::modulaName is empty");
				return;
			}
			std::string pureName = name;
			SetName(pureName + ".__runner__");
			pyBehavior = new py::object(PythonInterface::Import(moduleName)->attr("create")(this, pureName));
			state.FillTo(*pyBehavior);
			cppBehavior = dynamic_cast<PythonBehavior*>(GetLastChild());
			SPH_ASSERT_0(cppBehavior != nullptr);
			level->Attach(cppBehavior);
		});
	}
	

	void PythonBehaviorTrampoline::Init()
	{
		PythonBehavior::Init();
		PYBIND11_OVERRIDE_NAME(void, PythonBehavior, "sph_init", Init);
	}
	
	void PythonBehaviorTrampoline::Tick(float deltaTime)
	{
		PYBIND11_OVERRIDE_NAME(void, PythonBehavior, "tick", Tick, deltaTime);
	}

}