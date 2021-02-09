#include "editor_pch.h"
#include "EditorWindow.hpp"


BEGIN_EVENT_TABLE(Sharpheus::EditorWindow, wxFrame)
END_EVENT_TABLE()


namespace Sharpheus {

	EditorWindow::EditorWindow(const std::string& title, uint32_t width, uint32_t height)
		: wxFrame(nullptr, wxID_ANY, title, wxPoint(30, 30), wxSize(width, height))
	{
		creator = new Creator(this, wxPoint(0, 0), wxSize(200, 50));
		details = new DetailsPanel(this, wxPoint(0, 0), wxSize(190, 500));
		explorer = new Explorer(this, wxPoint(0, 0), wxSize(1040, 160));
		levelHierarchy = new LevelHierarchy(this, wxPoint(0, 0), wxSize(200, 610));
		toolBar = new ToolBar(this, wxPoint(0, 0), wxSize(840, 30));
		viewPort = new ViewPort(this, wxPoint(0, 0), wxSize(840, 460));

		sizer = new wxGridBagSizer(4, 4);
		sizer->Add(creator, wxGBPosition(0, 0), wxGBSpan(2, 1), wxEXPAND);
		sizer->Add(levelHierarchy, wxGBPosition(2, 0), wxGBSpan(2, 1), wxEXPAND);
		sizer->Add(toolBar, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND);
		sizer->Add(viewPort, wxGBPosition(1, 1), wxGBSpan(2, 1), wxEXPAND);
		sizer->Add(details, wxGBPosition(0, 2), wxGBSpan(3, 1), wxEXPAND);
		sizer->Add(explorer, wxGBPosition(3, 1), wxGBSpan(1, 2), wxEXPAND);

		sizer->AddGrowableCol(1);
		sizer->AddGrowableRow(2);
		sizer->AddGrowableRow(3);

		SetSizerAndFit(sizer);
	}


	EditorWindow::~EditorWindow()
	{

	}

}