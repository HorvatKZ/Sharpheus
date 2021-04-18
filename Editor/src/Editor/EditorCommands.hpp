#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class EditorCommands
	{
	public:
		static void Init(wxWindow* _editorWindow, std::function<void()>&& levelChangedCallback, std::function<void()>&& currChangedCallback);
		static void Clear();

		static void NewLevel();
		static bool LoadLevel();
		static bool LoadLevel(const wxString& levelPath);
		static bool SaveLevel();
		static bool SaveLevelAs();

		static bool AttachSceneToCurrent();
		static bool AttachSceneToCurrent(const wxString& scenePath);
		static bool SaveCurrentAsScene();

		static void CreateAnimation();
		static void EditAnimation(const wxString& animPath);

		static void ExportTheGame(bool includeLogging);

	private:
		static wxWindow* editorWindow;
		static std::function<void()> levelChangedCallback;
		static std::function<void()> currChangedCallback;

		static void HandleAnimationCreator(class AnimationCreatorDialog& creator);
	};

}