#include "editor_pch.h"
#include "EditorCommands.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/EditorWindow/EditorWindow.hpp"
#include "Editor/EditorWindow/MenuBar/LayerEditorDialog.hpp"
#include "Editor/EditorWindow/MenuBar/AnimationCreatorDialog.hpp"
#include "Editor/EditorWindow/MenuBar/TileSetCreatorDialog.hpp"
#include "Editor/Exporting/Exporter.hpp"
#include "Editor/TileMapEditor/TileMapEditor.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Engine/PythonInterface/PythonInterface.hpp"


namespace Sharpheus {

	std::function<void()> EditorCommands::levelChangedCallback;
	std::function<void()> EditorCommands::currChangedCallback;
	
	bool EditorCommands::isPlaying = false;
	TileMapEditor::Editor* EditorCommands::tme = nullptr;

	void EditorCommands::Init(std::function<void()>&& _levelChangedCallback, std::function<void()>&& _currChangedCallback)
	{
		levelChangedCallback = std::move(_levelChangedCallback);
		currChangedCallback = std::move(_currChangedCallback);
	}


	void EditorCommands::Clear()
	{
	}


	void EditorCommands::NewLevel()
	{
		if (!CheckPlayingAndTME("creating a new level")) {
			return;
		}

		SaveCurrentLevelIfNeeded();

		wxTextEntryDialog levelNameDialog(EditorData::GetMainWindow(), "Level name:", "New Level");
		levelNameDialog.SetTextValidator(wxFILTER_EMPTY);

		if (levelNameDialog.ShowModal() == wxID_CANCEL)
			return;

		ProjectData::CreateNewLevel(wxStr2StdStr(levelNameDialog.GetValue()));
		levelChangedCallback();
	}


	bool EditorCommands::LoadLevel()
	{
		if (!CheckPlayingAndTME("loading a level")) {
			return false;
		}

		SaveCurrentLevelIfNeeded();

		RelativeOpenDialog openDialog(EditorData::GetMainWindow(), "Open Level", ProjectData::GetPath() + "Levels\\", "Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus");

		if (!openDialog.Show())
			return true;

		bool success = ProjectData::GetProj()->LoadLevel(wxStr2StdStr(openDialog.GetPath()));
		SPHE_ASSERT(success, "Cannot load level. Check the log files for more information");
		levelChangedCallback();
		return success;
	}


	bool EditorCommands::LoadLevel(const wxString& levelPath)
	{
		if (!CheckPlayingAndTME("loading a level")) {
			return false;
		}

		SaveCurrentLevelIfNeeded();

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
		RelativeSaveDialog saveDialog(EditorData::GetMainWindow(), "Save Level", ProjectData::GetPath() + "Levels\\", "Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus", ProjectData::GetLevel()->GetName());

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

		RelativeOpenDialog openDialog(EditorData::GetMainWindow(), "Attach Scene", ProjectData::GetPath() + "Scenes\\", "Sharpheus scene file(*.scene.sharpheus) | *.scene.sharpheus");

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

		RelativeSaveDialog saveDialog(EditorData::GetMainWindow(), "Save as Scene", ProjectData::GetPath() + "Scenes\\", "Sharpheus scene file(*.scene.sharpheus) | *.scene.sharpheus");

		if (!saveDialog.Show())
			return true;

		bool success = ProjectData::GetLevel()->SaveAsScene(curr, wxStr2StdStr(saveDialog.GetPath()));
		SPHE_ASSERT(success, "Cannot save current GameObject as a scene. Check the log files for more information");
		return success;
	}


	void EditorCommands::LayerEditor()
	{
		LayerEditorDialog dial(EditorData::GetMainWindow());
		dial.ShowModal();
		currChangedCallback();
	}


	void EditorCommands::CreateAnimation()
	{
		AnimationCreatorDialog creator(EditorData::GetMainWindow());
		HandleAnimationCreator(creator);
	}


	void EditorCommands::EditAnimation(const wxString& animPath)
	{
		AnimationCreatorDialog creator(EditorData::GetMainWindow(), animPath);
		HandleAnimationCreator(creator);
	}


	void EditorCommands::CreateTileSet()
	{
		TileSetCreatorDialog creator(EditorData::GetMainWindow());
		HandleTileSetCreator(creator);
	}


	void EditorCommands::EditTileSet(const wxString& tileSetPath)
	{
		TileSetCreatorDialog creator(EditorData::GetMainWindow(), tileSetPath);
		HandleTileSetCreator(creator);
	}


	void EditorCommands::ReloadAssets()
	{
		if (!CheckPlayingAndTME("reloading assets")) {
			return;
		}

		SaveCurrentLevelIfNeeded("Reloading assets involves reloading the current level. Do you want to save before?");

		ResourceManager::Reload();
		((EditorWindow*)EditorData::GetMainWindow())->ReloadAssets();

		bool success = ProjectData::GetProj()->LoadLevel(wxStr2StdStr(ProjectData::GetLevel()->GetPath()));
		SPHE_ASSERT(success, "Cannot reload level. Check the log files for more information");
		levelChangedCallback();
	}


	void EditorCommands::ReloadScripts()
	{
		if (!CheckPlayingAndTME("reloading scripts")) {
			return;
		}

		SaveCurrentLevelIfNeeded("Reloading scripts involves reloading the current level. Do you want to save before?");

		PythonInterface::ReloadModules();

		bool success = ProjectData::GetProj()->LoadLevel(wxStr2StdStr(ProjectData::GetLevel()->GetPath()));
		SPHE_ASSERT(success, "Cannot reload level. Check the log files for more information");
		levelChangedCallback();
	}


	void EditorCommands::ExportTheGame(bool includeLogging)
	{
		SaveCurrentLevelIfNeeded("Do you want to save the current level before exporting?");

		Exporter exp(includeLogging);
		exp.Export();
	}


	void EditorCommands::OpenTileMapEditor(TileMap* tileMap)
	{
		if (!CheckPlayingAndTME("using the TileMap Editor")) {
			return;
		}

		if (tileMap == nullptr) {
			SPHE_ERROR("Cannot open TileMap Editor: The given TileMap is nullptr");
			return;
		}

		if (tileMap->GetTileSet() == nullptr) {
			SPHE_WARN("Please browse a TileSet before opening the TileMap Editor");
			return;
		}

		if (!tileMap->GetTileSet()->IsValid()) {
			SPHE_ERROR("Cannot open TileMap Editor: The given TileMap has an invalid TileSet");
			return;
		}

		tme = new TileMapEditor::Editor(EditorData::GetMainWindow(), EditorData::GetMainContext(), tileMap);
		tme->Bind(wxEVT_CLOSE_WINDOW, OnTMEClosed);
		tme->Show();
	}


	void EditorCommands::ChangeTileSet(GameObject* obj)
	{
		if (tme != nullptr) {
			tme->ChangeTileSet(obj);
		}
	}


	void EditorCommands::HandleAnimationCreator(AnimationCreatorDialog& creator)
	{
		if (creator.ShowModal() == wxID_CANCEL)
			return;

		const Image* atlas = ResourceManager::GetImage(wxStr2StdStr(creator.GetAtlas()), creator.UseFilter());
		Animation* anim = new Animation(atlas, creator.GetFrameWidth(), creator.GetFrameHeight(), creator.GetStartFrame(), creator.GetEndFrame());
		anim->SetName(wxStr2StdStr(creator.GetName()));
		anim->SetFrameTime(creator.GetFrameTime());
		bool success = anim->Save(wxStr2StdStr(creator.GetPath()));
		delete anim;

		SPHE_ASSERT(success, "Cannot save animation. Check the log files for more information");
	}


	void EditorCommands::HandleTileSetCreator(TileSetCreatorDialog& creator)
	{
		if (creator.ShowModal() == wxID_CANCEL)
			return;

		const Image* atlas = ResourceManager::GetImage(wxStr2StdStr(creator.GetAtlas()), creator.UseFilter());
		TileSet* tileSet = new TileSet(atlas, creator.GetFrameWidth(), creator.GetFrameHeight());
		tileSet->SetName(wxStr2StdStr(creator.GetName()));
		bool success = tileSet->Save(wxStr2StdStr(creator.GetPath()));
		delete tileSet;

		SPHE_ASSERT(success, "Cannot save animation. Check the log files for more information");
	}


	bool EditorCommands::CheckPlayingAndTME(const wxString& msg)
	{
		if (IsTMEActive()) {
			SPHE_WARN("Exit the TileMap Editor before " + wxStr2StdStr(msg));
			return false;
		}

		if (IsPlaying()) {
			SPHE_WARN("Exit game preview before " + wxStr2StdStr(msg));
			return false;
		}

		return true;
	}

	void EditorCommands::SaveCurrentLevelIfNeeded(const wxString& msg)
	{
		int response = wxMessageBox(msg, "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel();
		}
	}
}