#pragma once

#include "starter_pch.h"
#include "ProjectList.hpp"
#include <wx/srchctrl.h>


namespace Sharpheus {

	class StarterWindow : public wxFrame
	{
	public:
		StarterWindow(const std::string& title, uint32_t width, uint32_t height);
		virtual ~StarterWindow();

	private:
		wxStaticBitmap* header;
		wxButton* createButton;
		wxButton* browseButton;
		wxSearchCtrl* searchBox;
		ProjectList* projectList;

		void OnNew(wxCommandEvent& e);
		void OnBrowse(wxCommandEvent& e);
		void OnSearch(wxCommandEvent& e);
		void OnExitSearch(wxCommandEvent& e);

		static uint32_t border;
		static wxSize buttonSize;
	};

}