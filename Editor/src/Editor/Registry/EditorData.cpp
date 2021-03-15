#include "editor_pch.h"
#include "EditorData.hpp"
#include "ProjectData.hpp"
#include <wx/textfile.h>


namespace Sharpheus {

	GameObject* EditorData::curr = nullptr;
	wxString EditorData::path;


	void EditorData::Init(const wxString& configFile)
	{
		wxTextFile config;
		bool success = config.Open(configFile);
		SPHE_ASSERT(success, "Cannot find editor config file");
		path = config.GetFirstLine();
	}


	void EditorData::Clear()
	{
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