#pragma once

#include "editor_pch.h"
#include "DetailsPanel/DetailsPanel.hpp"
#include "Creator.hpp"
#include "Explorer.hpp"
#include "LevelHierarchy.hpp"
#include "ToolBar.hpp"
#include "ViewPort.hpp"
#include <wx/gbsizer.h>



namespace Sharpheus {

	class EditorWindow : public wxFrame
	{
	public:
		EditorWindow(const std::string& title, uint32_t width, uint32_t height);
		virtual ~EditorWindow();

	private:
		wxGridBagSizer* sizer;
		Creator* creator;
		DetailsPanel* details;
		Explorer* explorer;
		LevelHierarchy* levelHierarchy;
		ToolBar* toolBar;
		ViewPort* viewPort;

		DECLARE_EVENT_TABLE()
	};

}