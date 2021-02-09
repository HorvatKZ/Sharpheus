#include "editor_pch.h"
#include "EditorData.hpp"


namespace Sharpheus {

	Level* EditorData::level = nullptr;

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

}