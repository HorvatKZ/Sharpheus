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
		SPH_PROVIDE_SCRIPT(PythonRunnerBehavior, "Script", GetModuleName, SetModuleName, SetModuleNameFromScriptPath)
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


	PythonRunnerBehavior::~PythonRunnerBehavior()
	{
		SPH_PYEXEC("Deleting pyObject from " + moduleName, [&] {
			if (pyBehavior != nullptr) {
				while (pyBehavior->ref_count() != 1) {
					pyBehavior->dec_ref();
				}
				delete pyBehavior;
			}
		});
	}


	void PythonRunnerBehavior::SetModuleName(const std::string& moduleName)
	{
		if (pyBehavior != nullptr) {
			SPH_ERROR("PythonRunnerBehavior::SetModuleName call after instancing pyBehavior");
			return;
		}

		this->moduleName = moduleName;
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
		fl.Read(moduleName);
		return fl.GetStatus();
	}


	bool PythonRunnerBehavior::Save(FileSaver& fs)
	{
		Behavior::Save(fs);
		fs.Write(moduleName);
		return fs.GetStatus();
	}


	void PythonRunnerBehavior::CopyContent(GameObject* other)
	{
		SPH_COPY_HEADER(1, Behavior);
		PythonRunnerBehavior* trueOther = (PythonRunnerBehavior*)other;
		moduleName = trueOther->moduleName;
	}


	bool PythonRunnerBehavior::IsCompatibleWithParent(GameObject* parent)
	{
		return parent != nullptr;
	}


	void PythonRunnerBehavior::Init()
	{
		SPH_PYEXEC("Creating PythonBehavior from " + moduleName, [&] {
			if (moduleName.empty()) {
				SPH_WARN("PythonRunnerBehavior::modulaName is empty");
				return;
			}
			std::string pureName = name;
			SetName(pureName + ".__runner__");
			pyBehavior = new py::object(PythonInterface::Import(moduleName)->attr("create")(this, pureName));
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