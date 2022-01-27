#include "starter_pch.h"
#include <wx/textfile.h>
#include <wx/filename.h>
#include "ProjectList.hpp"
#include "ProcessControl.hpp"
#include "Engine/Project.hpp"
#include "Common/StringUtils.hpp"


namespace Sharpheus {

	uint32_t ProjectList::border = 10;
	uint32_t ProjectList::elemWidth = 150;
	uint32_t ProjectList::elemHeight = 125;

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

				Project* proj = new Project();
				proj->LoadProjectData(wxStr2StdStr(projectPath));
				projectVersions.push_back(proj->GetVersion());
				delete proj;

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
				DrawElem(dc, perRow, i, j);
				++j;
			}
		}
	}


	void ProjectList::DrawElem(wxDC& dc, uint32 perRow, uint32 ind, uint32 place)
	{
		wxString projName = projectNames[ind];
		uint32_t extent = GetTextExtent(projName).x;
		while (extent > elemWidth) {
			projName = projName.SubString(0, projName.Length() - 6) + "...";
			extent = GetTextExtent(projName).x;
		}

		dc.DrawBitmap(icon, wxPoint(
			border + place % perRow * (elemWidth + border) + (elemWidth - icon.GetWidth()) / 2,
			border + place / perRow * (elemHeight + border)
		));
		dc.DrawLabel(projectNames[ind], wxRect(
			place % perRow * (elemWidth + border) + border + (elemWidth - extent) / 2,
			place / perRow * (elemHeight + border) + border + icon.GetHeight(),
			extent, 22
		));

		const EngineVersion& projV = projectVersions[ind];
		extent = GetTextExtent(projV.GetVName()).x;
		uint32 halfborder = border / 2;
		dc.SetPen(*wxTRANSPARENT_PEN);
		if (projV.IsUnknown()) {
			dc.SetTextForeground(*wxBLACK);
			dc.SetBrush(wxBrush(wxColour(237, 85, 59)));
		} else if (projV < EngineVersion::latest) {
			dc.SetTextForeground(*wxBLACK);
			dc.SetBrush(wxBrush(wxColour(246, 213, 92)));
		} else if (projV == EngineVersion::latest) {
			dc.SetTextForeground(*wxBLACK);
			dc.SetBrush(wxBrush(wxColour(60, 174, 163)));
		} else {
			dc.SetTextForeground(*wxWHITE);
			dc.SetBrush(wxBrush(wxColour(32, 99, 155)));
		}
		dc.DrawRoundedRectangle(wxRect(
			place % perRow * (elemWidth + border) + 2 * border + halfborder,
			place / perRow * (elemHeight + border) + halfborder,
			extent + border, GetTextExtent(projV.GetVName()).y + border
		), halfborder);
		dc.DrawLabel(projV.GetVName(), wxRect(
			place % perRow * (elemWidth + border) + 3 * border,
			place / perRow * (elemHeight + border) + border,
			extent, 22
		));
		dc.SetTextForeground(*wxBLACK);
		dc.SetBrush(*wxWHITE_BRUSH);

		if (place == curr) {
			dc.DrawBitmap(delIcon, wxPoint(
				border + place % perRow * (elemWidth + border) + 105,
				border + place / perRow * (elemHeight + border) - 5
			));
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
					projectVersions.erase(projectVersions.begin() + i);
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
			const EngineVersion& projV = projectVersions[i - 1];
			if (projV.IsUnknown()) {
				wxMessageBox("Unable to open project\nThe project version is unknown", "Uknown project version", wxICON_ERROR | wxOK | wxCENTRE);
				return;
			} else if (projV < EngineVersion::latest) {
				int answer = wxMessageBox("This is a project with an older version. Opening it will result in undoable version conversion\nProceed?", "Version conversion", wxICON_INFORMATION | wxYES | wxNO | wxCENTRE);
				if (answer == wxNO) {
					return;
				}
			} else if (projV > EngineVersion::latest) {
				wxMessageBox("This is a project with a newer version\nPlease download a newer version of Sharpheus to open it", "Newer version", wxICON_WARNING | wxOK | wxCENTRE);
				return;
			}

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