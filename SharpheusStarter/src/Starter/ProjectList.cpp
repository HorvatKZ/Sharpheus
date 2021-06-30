#include "starter_pch.h"
#include <wx/textfile.h>
#include <wx/filename.h>
#include "ProjectList.hpp"
#include "ProcessControl.hpp"


namespace Sharpheus {

	uint32_t ProjectList::border = 10;

	ProjectList::ProjectList(wxWindow* parent, const wxPoint& pos, const wxSize& size, const wxString& projectsFile)
		: wxPanel(parent, wxID_ANY, pos, size), projectsFile(projectsFile)
	{
		Bind(wxEVT_PAINT, &ProjectList::OnPaintEvent, this);
		Bind(wxEVT_LEFT_DOWN, &ProjectList::OnClick, this);
		Bind(wxEVT_LEFT_DCLICK, &ProjectList::OnDoubleClick, this);
		Bind(wxEVT_MOTION, &ProjectList::OnMouseMove, this);

		wxTextFile file;
		if (!wxFileExists(projectsFile)) {
			file.Create(projectsFile);
		} else {
			file.Open(projectsFile);
		}
		wxString line = file.GetFirstLine();
		AddProject(line);
		
		while (!file.Eof()) {
			line = file.GetNextLine();
			AddProject(line);
		}

		icon = wxImage(ProcessControl::GetRoot() + "Assets\\Editor\\Icons\\sharpheus_icon.png", wxBITMAP_TYPE_PNG).Rescale(100, 100);
		delIcon = wxImage(ProcessControl::GetRoot() + "Assets\\Starter\\delete.png", wxBITMAP_TYPE_PNG);

		ProcessControl::SetProjectCreated(std::bind(&ProjectList::AppendProject, this, std::placeholders::_1));
	}


	ProjectList::~ProjectList()
	{
		wxTextFile file;
		file.Open(projectsFile);
		file.Clear();
		for (const wxString& project : projects) {
			file.AddLine(project);
		}
		file.Write();
	}


	void ProjectList::AppendProject(const wxString& projectPath)
	{
		uint32_t i = 0;
		while (i < projects.size()) {
			if (projects[i] == projectPath) {
				return;
			}
			++i;
		}

		AddProject(projectPath);
		Refresh();
	}


	void ProjectList::OnPaintEvent(wxPaintEvent& e)
	{
		wxPaintDC dc(this);
		Draw(dc);
	}


	void ProjectList::PaintNow()
	{
		wxClientDC dc(this);
		Draw(dc);
	}


	bool ProjectList::ShouldBeDisplayed(const wxString& name)
	{
		return searchStr.empty() || name.Length() >= searchStr.Length() && name.Left(searchStr.Length()) == searchStr;
	}


	void ProjectList::AddProject(const wxString& projectPath)
	{
		if (!projectPath.empty()) {
			if (::wxFileName::FileExists(projectPath)) {
				projects.Add(projectPath);
				wxString projectFile = projectPath.Mid(projectPath.find_last_of('\\') + 1);
				wxString projectName = projectFile.Left(projectFile.Length() - 15); // 15 = length of .proj.sharpheus
				projectNames.Add(projectName);
			}
			else {
				wxMessageBox("Error during initialization. " + projectPath + " cannot be located", "Missing project!", wxICON_ERROR | wxOK | wxCENTRE);
			}
		}
	}


	void ProjectList::Draw(wxDC& dc)
	{
		dc.Clear();
		
		if (projects.empty()) {
			wxString notFoundStr = "No previously opened project is found...";
			wxSize extent = GetTextExtent(notFoundStr);
			wxSize realSize = GetClientSize();
			dc.DrawLabel(notFoundStr, wxRect((realSize.x - extent.x) / 2, (realSize.y - extent.y) / 2, extent.x, extent.y));
			return;
		}

		uint32_t perRow = (GetClientSize().x - border) / (150 + border);
		uint32_t j = 0;
		for (uint32_t i = 0; i < projectNames.size(); ++i) {
			if (ShouldBeDisplayed(projectNames[i])) {
				uint32_t extent = GetTextExtent(projectNames[i]).x;
				dc.DrawBitmap(icon, wxPoint(border + j % perRow * (150 + border) + 25, border + j / perRow * (125 + border)));
				dc.DrawLabel(projectNames[i], wxRect(j % perRow * (150 + border) + border + (150 - extent) / 2, j / perRow * (125 + border) + border + 100, extent, 22));
				if (j == curr) {
					dc.DrawBitmap(delIcon, wxPoint(border + j % perRow * (150 + border) + 105, border + j / perRow * (125 + border) - 5));
				}
				++j;
			}
		}
	}


	void ProjectList::OnClick(wxMouseEvent& e)
	{
		if (curr == -1) {
			return;
		}

		wxPoint pos = e.GetPosition();
		uint32_t perRow = (GetClientSize().x - border) / (150 + border);
		wxPoint delta(pos.x - curr % perRow * (border + 150), pos.y - curr / perRow * (border + 125));

		if (border + 105 <= delta.x && delta.x <= border + 133 && border - 5 <= delta.y && delta.y <= border + 23) {
			uint32_t i = 0, j = 0;
			while (i < projectNames.size() && j < curr) {
				if (ShouldBeDisplayed(projectNames[i])) {
					++j;
				}
				++i;
			}
			if (j == curr) {
				int answer = wxMessageBox("Do you want to delete " + projectNames[i] + " project? This cannot be undone!", "Warning", wxICON_WARNING | wxYES | wxNO | wxCENTRE);
				if (answer == wxYES) {
					wxFileName::Rmdir(projects[i].Left(projects[i].find_last_of('\\')), wxPATH_RMDIR_RECURSIVE);
					projects.Remove(projects[i]);
					projectNames.Remove(projectNames[i]);
					Refresh();
				}
			}
		}
	}


	void ProjectList::OnDoubleClick(wxMouseEvent& e)
	{
		wxPoint pos = e.GetPosition();
		uint32_t col = pos.x / (border + 150);
		uint32_t row = pos.y / (border + 125);
		uint32_t perRow = (GetClientSize().x - border) / (150 + border);

		if (col >= perRow) {
			return;
		}

		uint32_t ind = row * perRow + col;
		uint32_t i = 0, j = 0;
		bool found = false;
		while (i < projects.size() && j <= ind) {
			if (ShouldBeDisplayed(projectNames[i])) {
				++j;
			}
			++i;
		}

		if (j > ind) {
			ProcessControl::OpenProj(projects[i - 1]);
		}
	}


	void ProjectList::OnMouseMove(wxMouseEvent& e)
	{
		wxPoint pos = e.GetPosition();
		uint32_t col = pos.x / (border + 150);
		uint32_t row = pos.y / (border + 125);
		uint32_t perRow = (GetClientSize().x - border) / (150 + border);

		if (col >= perRow) {
			curr = -1;
			return;
		}

		uint32_t prev = curr;
		curr = row * perRow + col;
		if (prev != curr) {
			Refresh();
		}
	}

}