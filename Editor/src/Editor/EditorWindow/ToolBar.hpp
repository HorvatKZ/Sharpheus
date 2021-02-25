#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class ToolBar : public wxPanel
	{
	public:
		ToolBar(wxFrame* parent, const wxPoint& pos, const wxSize& size, class ViewPort* viewPort);
		virtual ~ToolBar();

		void BindCallbacks(std::function<void()>&& startGame, std::function<void()>&& stopGame);
		void CancelPlay();

	private:
		wxBitmapButton* showHideColliders;
		wxBitmapButton* playButton;
		wxBitmapButton* gridButton;
		wxBitmapButton* snapToGridButton;

		std::function<void()> startGame, stopGame;
		wxBitmap shownColliders, hiddenColliders, play, stop, gridOn, gridOff, snapOn, snapOff;
		ViewPort* viewPort;
		bool isPlaying = false;

		void InitBitmaps();
		void OnShowHideCollidersPressed(wxCommandEvent& e);
		void OnPlayPressed(wxCommandEvent& e);
		void OnGridPressed(wxCommandEvent& e);
		void OnSnapToGridPressed(wxCommandEvent& e);
	};

}