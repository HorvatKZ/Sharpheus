#pragma once

#include "editor_pch.h"
#include "DetailsPanel/DetailsPanel.hpp"
#include "Creator/Creator.hpp"
#include "Explorer.hpp"
#include "LevelHierarchy.hpp"
#include "ToolBar.hpp"
#include "ViewPort/ViewPort.hpp"
#include "MenuBar/MenuBar.hpp"
#include "Editor/GamePreview/GamePreview.hpp"
#include <wx/gbsizer.h>



namespace Sharpheus {

	class EditorWindow : public wxFrame
	{
	public:
		EditorWindow(const std::string& title, uint32_t width, uint32_t height);
		virtual ~EditorWindow();

		void InitContent();
		void LevelChanged();
		void CurrentChanged();
		void CurrentNameChanged(const std::string& oldName, const std::string& newName);
		void CurrentDataChanged();
		void BehaviorChanged(uint32_t subType);
		void StartGame(bool withCurrent);
		void StopGame();

	private:
		wxGridBagSizer* sizer;
		Creator* creator;
		DetailsPanel* details;
		Explorer* explorer;
		LevelHierarchy* levelHierarchy;
		ToolBar* toolBar;
		ViewPort* viewPort;
		MenuBar* menuBar;

		GamePreview* game = nullptr;
		Camera* originalCamera = nullptr;
		std::queue<uint32_t> behviorChangeRequests;

		void OnGamePreviewExit(wxCloseEvent& e);
		void OnClose(wxCloseEvent& e);
		void OnIdle(wxIdleEvent& e);

		void ChangeBehavior(uint32_t subType);
	};

}