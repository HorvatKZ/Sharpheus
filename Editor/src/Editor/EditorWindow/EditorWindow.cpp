#include "editor_pch.h"
#include "EditorWindow.hpp"
#include "Editor/Registry/EditorData.hpp"


namespace Sharpheus {

	EditorWindow::EditorWindow(const std::string& title, uint32_t width, uint32_t height)
		: wxFrame(nullptr, wxID_ANY, title, wxPoint(30, 30), wxSize(width, height))
	{
		details = new DetailsPanel(this, wxPoint(0, 0), wxSize(240, 500));
		explorer = new Explorer(this, wxPoint(0, 0), wxSize(1040, 220));
		levelHierarchy = new LevelHierarchy(this, wxPoint(0, 0), wxSize(200, 610));
		viewPort = new ViewPort(this, wxPoint(0, 0), wxSize(790, 400));

		creator = new Creator(this, wxPoint(0, 0), wxSize(200, 50), levelHierarchy);
		toolBar = new ToolBar(this, wxPoint(0, 0), wxSize(144, 30), viewPort);

		levelHierarchy->BindCallbacks(SPH_BIND_THIS_0(EditorWindow::CurrentChanged));
		details->BindCallbacks(SPH_BIND_THIS_2(EditorWindow::CurrentNameChanged), SPH_BIND_THIS_0(EditorWindow::CurrentDataChanged));
		viewPort->BindCallbacks(SPH_BIND_THIS_0(EditorWindow::CurrentChanged), SPH_BIND_THIS_0(EditorWindow::CurrentDataChanged));
		toolBar->BindCallbacks(SPH_BIND_THIS_0(EditorWindow::StartGame), SPH_BIND_THIS_0(EditorWindow::StopGame));

		sizer = new wxGridBagSizer(4, 4);
		sizer->Add(creator, wxGBPosition(0, 0), wxGBSpan(2, 1), wxEXPAND);
		sizer->Add(levelHierarchy, wxGBPosition(2, 0), wxGBSpan(2, 1), wxEXPAND);
		sizer->Add(toolBar, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALIGN_CENTER);
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


	void EditorWindow::InitContent()
	{
		levelHierarchy->FillWith(EditorData::GetLevel()->GetRoot());
	}


	void EditorWindow::CurrentChanged()
	{
		GameObject* curr = EditorData::GetCurrent();
		details->CurrentChanged(curr);
		levelHierarchy->CurrentChanged(curr);
		viewPort->Refresh();
	}


	void EditorWindow::CurrentNameChanged(const std::string& oldName, const std::string& newName)
	{
		levelHierarchy->CurrentNameChanged(oldName, newName);
		details->CurrentNameChanged(oldName, newName);
	}


	void EditorWindow::CurrentDataChanged()
	{
		GameObject* curr = EditorData::GetCurrent();
		details->CurrentDataChanged(curr);
		viewPort->Refresh();
	}


	void EditorWindow::StartGame()
	{
		viewPort->SetPlaying(true);
		delete game;
		game = new GamePreview(EditorData::GetLevel());
		game->Run();
	}


	void EditorWindow::StopGame()
	{
		viewPort->SetPlaying(false);
		delete game;
		game = nullptr;
	}

}