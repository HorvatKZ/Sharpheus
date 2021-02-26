#include "editor_pch.h"
#include "EditorWindow.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


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

		menuBar = new MenuBar();
		menuBar->BindCallbacks(SPH_BIND_THIS_0(EditorWindow::LevelChanged));
		SetMenuBar(menuBar);

		ProjectData::SetWinProps(Window::Props());

		wxIcon icon;
		icon.CopyFromBitmap(wxBitmap("../Assets/Editor/Icons/sharpheus_icon.png", wxBITMAP_TYPE_PNG));
		SetIcon(icon);
	}


	EditorWindow::~EditorWindow()
	{

	}


	void EditorWindow::InitContent()
	{
		levelHierarchy->FillWith(EditorData::GetLevel()->GetRoot());
	}


	void EditorWindow::LevelChanged()
	{
		levelHierarchy->FillWith(EditorData::GetLevel()->GetRoot());
		viewPort->Refresh();
		details->CurrentChanged(nullptr);
		EditorData::SetCurrent(nullptr);
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
		bool success;
		if (!EditorData::GetLevel()->HasPath()) {
			wxMessageBox("Level needs to be saved before starting the GamePreview", "Warning", wxICON_WARNING | wxOK | wxCENTRE);

			wxFileDialog saveDialog(this, "Save Level", "../Levels", "",
				"Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

			if (saveDialog.ShowModal() == wxID_CANCEL) {
				toolBar->CancelPlay();
				return;
			}

			success = EditorData::GetLevel()->Save(wxStr2StdStr(saveDialog.GetPath()));
		} else {
			success = EditorData::GetLevel()->Save();
		}

		if (!success) {
			SPHE_ERROR("Cannot save level. Check the log files for more information");
			toolBar->CancelPlay();
		}

		viewPort->SetPlaying(true);
		originalCamera = Renderer::GetCamera();
		delete game;
		game = new GamePreview(this, EditorData::GetLevel()->GetPath(), viewPort->GetContext(), ProjectData::GetWinProps());
		game->Bind(wxEVT_CLOSE_WINDOW, &EditorWindow::OnGamePreviewExit, this);
		game->Show();
	}


	void EditorWindow::StopGame()
	{
		viewPort->SetPlaying(false);
		game->Destroy();
		game = nullptr;
		originalCamera->SetCurrent();
		
		Window::Props props = ProjectData::GetWinProps();
		Camera::SetStaticRect(props.width, props.height);
	}


	void EditorWindow::OnGamePreviewExit(wxCloseEvent& e)
	{
		toolBar->CancelPlay();
		StopGame();
	}

}