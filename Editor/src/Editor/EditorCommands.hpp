#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	namespace TileMapEditor {
		class Editor;
	}


	class EditorCommands
	{
	public:
		static void Init(std::function<void()>&& levelChangedCallback, std::function<void()>&& currChangedCallback);
		static void Clear();

		static void NewLevel();
		static bool LoadLevel();
		static bool LoadLevel(const wxString& levelPath);
		static bool SaveLevel();
		static bool SaveLevelAs();

		static bool AttachSceneToCurrent();
		static bool AttachSceneToCurrent(const wxString& scenePath);
		static bool SaveCurrentAsScene();

		static void LayerEditor();

		static void CreateAnimation();
		static void EditAnimation(const wxString& animPath);

		static void CreateTileSet();
		static void EditTileSet(const wxString& tileSetPath);

		static void ReloadAssets();
		static void ReloadScripts();

		static void ExportTheGame(bool includeLogging);

		static void OpenTileMapEditor(TileMap* tileMap);
		static void ChangeTileSet(GameObject* obj);

		static inline void SetPlaying(bool playing) { isPlaying = playing; }
		static inline bool IsPlaying() { return isPlaying; }
		static inline bool IsTMEActive() { return tme != nullptr; }

	private:
		static bool isPlaying;
		static TileMapEditor::Editor* tme;
		static std::function<void()> levelChangedCallback;
		static std::function<void()> currChangedCallback;

		static void HandleAnimationCreator(class AnimationCreatorDialog& creator);
		static void HandleTileSetCreator(class TileSetCreatorDialog& creator);

		static inline void OnTMEClosed(wxCloseEvent& e) { tme = nullptr; e.Skip(); }

		static bool CheckPlayingAndTME(const wxString& msg);
		static void SaveCurrentLevelIfNeeded(const wxString& msg = "Do you want to save the current level?");
	};

}