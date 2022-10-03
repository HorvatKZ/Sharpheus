#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"
#include "LocalListeners/ControlListener.hpp"
#include "LocalListeners/RenderableListener.hpp"
#include "Engine/PythonInterface/PythonInterface.hpp"
#include "Engine/PythonInterface/PythonObjectState.hpp"


namespace Sharpheus {

	class SPH_EXPORT PythonBehavior : public Behavior, public RenderableListener, public ColliderListener, public ControlListener
	{
	public:
		PythonBehavior(GameObject* parent, const std::string& name) : Behavior(parent, name),
			RenderableListener(GetID()), ColliderListener(GetID()), ControlListener(GetID()) {}
		virtual ~PythonBehavior() = default;

		SPH_DECL_GAMEOBJECT(PythonBehavior);
		SPH_DECL_BEHAVIOR(PythonBehavior, 1);
	};


	class SPH_EXPORT PythonRunnerBehavior : public Behavior
	{
	public:
		PythonRunnerBehavior(GameObject* parent, const std::string& name);
		virtual ~PythonRunnerBehavior();

		inline const std::string& GetModuleName() { return moduleName; }
		void SetModuleName(const std::string& moduleName);
		void SetModuleNameFromScriptPath(const std::string& scriptPath);

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(PythonRunnerBehavior);

		// Simplified SPH_DECL_BEHAVIOR
		virtual inline uint32 GetSubType() override { return 1; }
		virtual inline ClassInfoPtr GetBehaviorClassInfo() override;
		virtual void Init() override;
		static bool IsCompatibleWithParent(GameObject* parent);

	protected:
		std::string moduleName;

		py::object* pyBehavior = nullptr;
		PythonBehavior* cppBehavior = nullptr;

		PythonObjectState state;
		bool objectStateInited = false;

		ClassInfoPtr objInfo = nullptr;
		std::string objInfosModuleName;


		virtual bool Save(FileSaver& fs) override;

		virtual void CopyContent(GameObject* other) override;

		void InitObjectStateIfNeeded();
	};


	class PythonBehaviorTrampoline : public PythonBehavior
	{
	public:
		using PythonBehavior::PythonBehavior;
		virtual ~PythonBehaviorTrampoline() = default;

		void Init() override;

	protected:
		void Tick(float deltaTime) override;
	};
}