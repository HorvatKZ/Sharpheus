#include "editor_pch.h"
#include "EditorWindow.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "BehaviorCreator.hpp"


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
		details->BindCallbacks(SPH_BIND_THIS_2(EditorWindow::CurrentNameChanged), SPH_BIND_THIS_0(EditorWindow::CurrentDataChanged), SPH_BIND_THIS_1(EditorWindow::BehaviorChanged));
		viewPort->BindCallbacks(SPH_BIND_THIS_0(EditorWindow::CurrentChanged), SPH_BIND_THIS_0(EditorWindow::CurrentDataChanged));
		toolBar->BindCallbacks(SPH_BIND_THIS_1(EditorWindow::StartGame), SPH_BIND_THIS_0(EditorWindow::StopGame));

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

		menuBar = new MenuBar(this);
		menuBar->BindCallbacks(SPH_BIND_THIS_0(EditorWindow::LevelChanged));
		SetMenuBar(menuBar);

		wxIcon icon;
		icon.CopyFromBitmap(wxBitmap(EditorData::GetPath() + "Assets\\Editor\\Icons\\sharpheus_icon.png", wxBITMAP_TYPE_PNG));
		SetIcon(icon);

		Bind(wxEVT_IDLE, &EditorWindow::OnIdle, this);
	}


	EditorWindow::~EditorWindow()
	{

	}


	void EditorWindow::InitContent()
	{
		levelHierarchy->FillWith(ProjectData::GetLevel()->GetRoot());
		viewPort->InitEditingArrow();
	}


	void EditorWindow::LevelChanged()
	{
		levelHierarchy->FillWith(ProjectData::GetLevel()->GetRoot());
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


	void EditorWindow::BehaviorChanged(uint32_t subType)
	{
		behviorChangeRequests.push(subType);
	}


	void EditorWindow::StartGame(bool withCurrent)
	{
		bool success;
		if (!ProjectData::GetLevel()->HasPath()) {
			wxMessageBox("Level needs to be saved before starting the GamePreview", "Warning", wxICON_WARNING | wxOK | wxCENTRE);

			RelativeSaveDialog saveDialog(this, "Save Level", ProjectData::GetPath() + "Levels\\", "Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus");

			if (!saveDialog.Show()) {
				toolBar->CancelPlay();
				return;
			}

			success = ProjectData::GetProj()->SaveLevel(wxStr2StdStr(saveDialog.GetPath()));
		} else {
			success = ProjectData::GetLevel()->Save();
		}

		if (!success) {
			SPHE_ERROR("Cannot save level. Check the log files for more information");
			toolBar->CancelPlay();
		}

		viewPort->SetPlaying(true);
		originalCamera = Renderer::GetCamera();
		delete game;
		game = new GamePreview(this, withCurrent ? ProjectData::GetLevel()->GetFullPath() : ProjectData::GetProjectPath(),
			viewPort->GetContext(), ProjectData::GetWinProps());
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

	void EditorWindow::OnIdle(wxIdleEvent& e)
	{
		if (!behviorChangeRequests.empty()) {
			ChangeBehavior(behviorChangeRequests.front());
			behviorChangeRequests.pop();
		}
		e.Skip();
	}


	void EditorWindow::ChangeBehavior(uint32_t subType)
	{
		GameObject* curr = EditorData::GetCurrent();
		if (!BehaviorCreator::IsCompatibleWithParent(subType, curr->GetParent())) {
			SPHE_ERROR("This Behavior type is not compatible with its parent!");
			return;
		}

		if (curr->GetType() != GameObject::Type::Behavior) {
			SPHE_ERROR("Wrong signal... The current GameObject is not a Behavior");
			return;
		}

		if (((Behavior*)curr)->GetSubType() != 0) {
			SPHE_ERROR("Wrong signal... The current Behavior is not a PlaceholderBehavior, so its type cannot be changed");
			return;
		}

		GameObject* behavior = BehaviorCreator::Create(subType, (PlaceholderBehavior*)curr);
		EditorData::SetCurrent(behavior);
		CurrentChanged();
	}

}