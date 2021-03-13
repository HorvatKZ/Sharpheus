#include "editor_pch.h"
#include "EditorData.hpp"
#include "ProjectData.hpp"


namespace Sharpheus {

	GameObject* EditorData::curr = nullptr;
	std::string EditorData::path;


	void EditorData::Init(const std::string& _path)
	{
		path = _path;
	}


	void EditorData::Clear()
	{
	}


	void EditorData::SetCurrent(const std::string& currName)
	{
		curr = ProjectData::GetLevel()->GetGameObject(currName);
	}


	void EditorData::SetCurrent(GameObject* _curr)
	{
		curr = _curr;
	}

}