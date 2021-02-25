#include "editor_pch.h"
#include "ToolBar.hpp"
#include "ViewPort/ViewPort.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include "Engine/CollisionSystem/CollisionSystem.hpp"
#include "Editor/Registry/EditorData.hpp"


namespace Sharpheus {

	ToolBar::ToolBar(wxFrame* parent, const wxPoint& pos, const wxSize& size, ViewPort* viewPort)
		: wxPanel(parent, wxID_ANY, pos, size), viewPort(viewPort)
	{
		InitBitmaps();

		showHideColliders = new wxBitmapButton(this, wxID_ANY, hiddenColliders, wxPoint(3, 3), wxSize(24, 24));
		playButton = new wxBitmapButton(this, wxID_ANY, play, wxPoint(59, 2), wxSize(26, 26));
		gridButton = new wxBitmapButton(this, wxID_ANY, gridOn, wxPoint(90, 3), wxSize(24, 24));;
		snapToGridButton = new wxBitmapButton(this, wxID_ANY, snapOff, wxPoint(117, 3), wxSize(24, 24));

		showHideColliders->Bind(wxEVT_BUTTON, &ToolBar::OnShowHideCollidersPressed, this);
		playButton->Bind(wxEVT_BUTTON, &ToolBar::OnPlayPressed, this);
		gridButton->Bind(wxEVT_BUTTON, &ToolBar::OnGridPressed, this);
		snapToGridButton->Bind(wxEVT_BUTTON, &ToolBar::OnSnapToGridPressed, this);
	}


	ToolBar::~ToolBar()
	{
	}


	void ToolBar::BindCallbacks(std::function<void()>&& startGame, std::function<void()>&& stopGame)
	{
		this->startGame = std::move(startGame);
		this->stopGame = std::move(stopGame);
	}


	void ToolBar::CancelPlay()
	{
		isPlaying = false;
		playButton->SetBitmap(play);
	}


	void ToolBar::InitBitmaps()
	{
		shownColliders = ImageManager::GetImage("shown_colliders.png", ImageManager::PathType::TOOLBAR);
		hiddenColliders = ImageManager::GetImage("hidden_colliders.png", ImageManager::PathType::TOOLBAR);
		play = ImageManager::GetImage("play.png", ImageManager::PathType::TOOLBAR);
		stop = ImageManager::GetImage("stop.png", ImageManager::PathType::TOOLBAR);
		gridOn = ImageManager::GetImage("grid_on.png", ImageManager::PathType::TOOLBAR);
		gridOff = ImageManager::GetImage("grid_off.png", ImageManager::PathType::TOOLBAR);
		snapOn = ImageManager::GetImage("snaptogrid_on.png", ImageManager::PathType::TOOLBAR);
		snapOff = ImageManager::GetImage("snaptogrid_off.png", ImageManager::PathType::TOOLBAR);
	}


	void ToolBar::OnShowHideCollidersPressed(wxCommandEvent& e)
	{
		if (EditorData::GetLevel()->GetCollSys().AreCollidersVisible()) {
			EditorData::GetLevel()->GetCollSys().HideColliders();
			showHideColliders->SetBitmap(hiddenColliders);
		} else {
			EditorData::GetLevel()->GetCollSys().ShowColliders();
			showHideColliders->SetBitmap(shownColliders);
		}
		viewPort->Refresh();
	}


	void ToolBar::OnPlayPressed(wxCommandEvent& e)
	{
		if (isPlaying) {
			isPlaying = false;
			playButton->SetBitmap(play);
			stopGame();
		} else {
			isPlaying = true;
			playButton->SetBitmap(stop);
			startGame();
		}
	}


	void ToolBar::OnGridPressed(wxCommandEvent& e)
	{
		if (viewPort->IsGridOn()) {
			viewPort->SetGrid(false);
			gridButton->SetBitmap(gridOff);
			snapToGridButton->Enable(false);
		} else {
			viewPort->SetGrid(true);
			gridButton->SetBitmap(gridOn);
			snapToGridButton->Enable(true);
		}
		viewPort->Refresh();
	}


	void ToolBar::OnSnapToGridPressed(wxCommandEvent& e)
	{
		if (viewPort->IsSnapToGrid()) {
			viewPort->SetSnapToGrid(false);
			snapToGridButton->SetBitmap(snapOff);
		}
		else {
			viewPort->SetSnapToGrid(true);
			snapToGridButton->SetBitmap(snapOn);
		}
		viewPort->Refresh();
	}

}