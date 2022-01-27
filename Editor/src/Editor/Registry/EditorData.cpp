#include "editor_pch.h"
#include "EditorData.hpp"
#include "ProjectData.hpp"
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/textfile.h>
#include <wx/stdpaths.h>


namespace Sharpheus {

	GameObject* EditorData::curr = nullptr;
	wxString EditorData::path;
	wxString EditorData::configFile;
	EditorData::GridProps EditorData::gridProps;
	wxString EditorData::version(EngineVersion::latest.GetVName());
	wxString EditorData::oglVersion("Not determined");
	wxFrame* EditorData::editorWindow = nullptr;
	wxGLContext* EditorData::mainContext = nullptr;


	void EditorData::Init()
	{
		configFile = wxStandardPaths::Get().GetUserConfigDir() + "\\Sharpheus\\editorConfig.txt";
		if (!wxFileExists(configFile)) {
			SPHE_ERROR("The config file does not exist. Please run the SharpheusStarter to resolve this issue");
			wxExit();
		}

		wxFileInputStream file(configFile);
		wxTextInputStream config(file);
		path = config.ReadLine();
		gridProps.size = config.ReadDouble();
		gridProps.thickness = config.ReadDouble();
		wxChar c = config.GetChar();
		gridProps.isInForeground = (c == 't');
		config.GetChar();
		gridProps.color.r = config.Read8();
		gridProps.color.g = config.Read8();
		gridProps.color.b = config.Read8();
		gridProps.color.a = config.Read8();
		c = config.GetChar();
		gridProps.isCamOutlineVisible = (c == 't');
		config.GetChar();
		gridProps.camOutlineThickness = config.ReadDouble();
		gridProps.camOutlineColor.r = config.Read8();
		gridProps.camOutlineColor.g = config.Read8();
		gridProps.camOutlineColor.b = config.Read8();
		gridProps.camOutlineColor.a = config.Read8();
	}


	void EditorData::Clear()
	{
		wxFileOutputStream file(configFile);
		wxTextOutputStream config(file);
		config << path << "\n";
		config << gridProps.size << " " << gridProps.thickness << (gridProps.isInForeground ? " t " : " f ") <<
			gridProps.color.r << " " << gridProps.color.g << " " << gridProps.color.b << " " << gridProps.color.a << "\n";
		config << (gridProps.isCamOutlineVisible ? "t " : "f ") << gridProps.camOutlineThickness << " " <<
			gridProps.camOutlineColor.r << " " << gridProps.camOutlineColor.g << " " << gridProps.camOutlineColor.b << " " << gridProps.camOutlineColor.a << "\n";
	}


	void EditorData::SetCurrent(const wxString& currName)
	{
		curr = ProjectData::GetLevel()->GetGameObject(wxStr2StdStr(currName));
	}


	void EditorData::SetCurrent(GameObject* _curr)
	{
		curr = _curr;
	}

}