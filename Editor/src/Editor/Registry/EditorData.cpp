#include "editor_pch.h"
#include "EditorData.hpp"
#include "ProjectData.hpp"
#include <wx/txtstrm.h>
#include <wx/wfstream.h>


namespace Sharpheus {

	GameObject* EditorData::curr = nullptr;
	wxString EditorData::path;
	wxString EditorData::configFile;
	EditorData::GridProps EditorData::gridProps;


	void EditorData::Init(const wxString& _configFile)
	{
		configFile = _configFile;
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
	}


	void EditorData::Clear()
	{
		wxFileOutputStream file(configFile);
		wxTextOutputStream config(file);
		config << path << "\n";
		config << gridProps.size << " " << gridProps.thickness << (gridProps.isInForeground ? " t " : " f ") <<
			gridProps.color.r << " " << gridProps.color.g << " " << gridProps.color.b << " " << gridProps.color.a << "\n";
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