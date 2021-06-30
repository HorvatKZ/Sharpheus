#include "editor_pch.h"
#include "EditorCommands.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Editor/EditorWindow/MenuBar/AnimationCreatorDialog.hpp"
#include "Editor/Exporting/Exporter.hpp"


namespace Sharpheus {

	wxWindow* EditorCommands::editorWindow = nullptr;
	std::function<void()> EditorCommands::levelChangedCallback;
	std::function<void()> EditorCommands::currChangedCallback;


	void EditorCommands::Init(wxWindow* _editorWindow, std::function<void()>&& _levelChangedCallback, std::function<void()>&& _currChangedCallback)
	{
		editorWindow = _editorWindow;
		levelChangedCallback = std::move(_levelChangedCallback);
		currChangedCallback = std::move(_currChangedCallback);
	}


	void EditorCommands::Clear()
	{
	}


	void EditorCommands::NewLevel()
	{
		int response = wxMessageBox("Do you want to save the current level?", "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel();
		}

		wxTextEntryDialog levelNameDialog(editorWindow, "Level name:", "New Level");
		levelNameDialog.SetTextValidator(wxFILTER_EMPTY);

		if (levelNameDialog.ShowModal() == wxID_CANCEL)
			return;

		ProjectData::CreateNewLevel(wxStr2StdStr(levelNameDialog.GetValue()));
		levelChangedCallback();
	}


	bool EditorCommands::LoadLevel()
	{
		int response = wxMessageBox("Do you want to save the current level?", "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel();
		}

		RelativeOpenDialog openDialog(editorWindow, "Open Level", ProjectData::GetPath() + "Levels\\", "Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus");

		if (!openDialog.Show())
			return true;

		bool success = ProjectData::GetProj()->LoadLevel(wxStr2StdStr(openDialog.GetPath()));
		SPHE_ASSERT(success, "Cannot load level. Check the log files for more information");
		levelChangedCallback();
		return success;
	}


	bool EditorCommands::LoadLevel(const wxString& levelPath)
	{
		int response = wxMessageBox("Do you want to save the current level?", "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel();
		}

		bool success = ProjectData::GetProj()->LoadLevel(wxStr2StdStr(levelPath));
		SPHE_ASSERT(success, "Cannot load level. Check the log files for more information");
		levelChangedCallback();
		return success;
	}


	bool EditorCommands::SaveLevel()
	{
		if (ProjectData::GetLevel()->HasPath()) {
			bool success = ProjectData::GetLevel()->Save();
			SPHE_ASSERT(success, "Cannot save level. Check the log files for more information");
			return success;
		} else {
			return SaveLevelAs();
		}
	}


	bool EditorCommands::SaveLevelAs()
	{
		RelativeSaveDialog saveDialog(editorWindow, "Save Level", ProjectData::GetPath() + "Levels\\", "Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus");

		if (!saveDialog.Show())
			return true;

		bool success = ProjectData::GetProj()->SaveLevel(wxStr2StdStr(saveDialog.GetPath()));
		SPHE_ASSERT(success, "Cannot save level. Check the log files for more information");
		return success;
	}


	bool EditorCommands::AttachSceneToCurrent()
	{
		GameObject* curr = EditorData::GetCurrent();
		if (curr == nullptr) {
			SPHE_ERROR("No GameObject is selected for attaching");
			return false;
		}

		RelativeOpenDialog openDialog(editorWindow, "Attach Scene", ProjectData::GetPath() + "Scenes\\", "Sharpheus scene file(*.scene.sharpheus) | *.scene.sharpheus");

		if (!openDialog.Show())
			return true;

		return AttachSceneToCurrent(openDialog.GetPath());
	}


	bool EditorCommands::AttachSceneToCurrent(const wxString& scenePath)
	{
		GameObject* curr = EditorData::GetCurrent();
		if (curr == nullptr) {
			SPHE_ERROR("No GameObject is selected for attaching");
			return false;
		}

		bool success = ProjectData::GetLevel()->AttachSceneTo(curr, wxStr2StdStr(scenePath));
		SPHE_ASSERT(success, "Cannot save level. Check the log files for more information");
		
		levelChangedCallback();
		EditorData::SetCurrent(curr->GetLastChild());
		currChangedCallback();
		return success;
	}


	bool EditorCommands::SaveCurrentAsScene()
	{
		GameObject* curr = EditorData::GetCurrent();
		if (curr == nullptr) {
			SPHE_ERROR("No GameObject is selected to be saved as a scene");
			return false;
		}

		RelativeSaveDialog saveDialog(editorWindow, "Save as Scene", ProjectData::GetPath() + "Scenes\\", "Sharpheus scene file(*.scene.sharpheus) | *.scene.sharpheus");

		if (!saveDialog.Show())
			return true;

		bool success = ProjectData::GetLevel()->SaveAsScene(curr, wxStr2StdStr(saveDialog.GetPath()));
		SPHE_ASSERT(success, "Cannot save current GameObject as a scene. Check the log files for more information");
		return success;
	}


	void EditorCommands::CreateAnimation()
	{
		AnimationCreatorDialog creator(editorWindow);
		HandleAnimationCreator(creator);
	}


	void EditorCommands::EditAnimation(const wxString& animPath)
	{
		AnimationCreatorDialog creator(editorWindow, animPath);
		HandleAnimationCreator(creator);
	}


	void EditorCommands::ExportTheGame(bool includeLogging)
	{
		int response = wxMessageBox("Do you want to save the current level before exporting?", "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel();
		}

		Exporter exp(includeLogging);
		exp.Export();
	}


	void EditorCommands::HandleAnimationCreator(AnimationCreatorDialog& creator)
	{
		if (creator.ShowModal() == wxID_CANCEL)
			return;

		Image* atlas = ResourceManager::GetImage(wxStr2StdStr(creator.GetAtlas()), false);
		Animation* anim = new Animation(atlas, creator.GetFrameWidth(), creator.GetFrameHeight(), creator.GetStartFrame(), creator.GetEndFrame());
		anim->SetName(wxStr2StdStr(creator.GetName()));
		anim->SetFrameTime(creator.GetFrameTime());
		bool success = anim->Save(wxStr2StdStr(creator.GetPath()));
		delete anim;

		SPHE_ASSERT(success, "Cannot save animation. Check the log files for more information");
	}
}