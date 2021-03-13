#pragma once

#include "starter_pch.h"


namespace Sharpheus {

	class ProjectList : public wxPanel
	{
	public:
		ProjectList(wxWindow* parent, const wxPoint& pos, const wxSize& size, const wxString& projectsFile);
		virtual ~ProjectList();

		inline void Search(const wxString& searchStr) {
			this->searchStr = searchStr;
			Refresh();
		}
		inline void ExitSearch() {
			this->searchStr = "";
			Refresh();
		}

		void AppendProject(const wxString& projectPath);

		void OnPaintEvent(wxPaintEvent& e);
		void PaintNow();

	private:
		wxBitmap icon, delIcon;
		wxString projectsFile, searchStr;
		wxArrayString projects, projectNames;
		int32_t curr = -1;

		static uint32_t border;

		bool ShouldBeDisplayed(const wxString& name);
		void AddProject(const wxString& projectPath);
		void Draw(wxDC& dc);

		void OnClick(wxMouseEvent& e);
		void OnDoubleClick(wxMouseEvent& e);
		void OnMouseMove(wxMouseEvent& e);
	};

}