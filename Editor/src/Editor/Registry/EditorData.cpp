#include "editor_pch.h"
#include "EditorData.hpp"


namespace Sharpheus {

	Level* EditorData::level = nullptr;
	GameObject* EditorData::curr = nullptr;


	void EditorData::Init()
	{
	}

	void EditorData::Clear()
	{
		delete level;
	}

	void EditorData::SetLevel(Level* _level)
	{
		if (_level != level) {
			delete level;
		}

		level = _level;
	}

	void EditorData::SetCurrent(const std::string& currName)
	{
		curr = level->GetGameObject(currName);
	}

	void EditorData::SetCurrent(GameObject* _curr)
	{
		curr = _curr;
	}

}